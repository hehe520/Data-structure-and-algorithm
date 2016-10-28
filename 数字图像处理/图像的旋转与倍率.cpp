
// 图像的旋转与倍率, 原理见报告
// 编译环境 VC++6.0, C++编译

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <math.h>

#define NUM 192			// 无图的灰度值
#define PI  3.1415926	// PI
typedef unsigned char UC;
typedef unsigned long int ULI;

typedef struct		// ppm 图像
{
	char type;		// P 标志
	char flag;		// 几号
	char byte;		// 每个像素占用字节数
	int  width;		// 宽
	int  height;	// 长
	int  maxv;		// 最大量化值
	UC   *data;		// 图像数据
}IMG;

const double EPS = 1e-6;				// 误差
int open_img(IMG *ppm, char *path);		// 打开
int save_img(IMG *ppm, char *path);		// 保存
int copy_img(IMG *d, IMG const *s);		// 复制
int close_img(IMG *ppm);				// 关闭
int extract_rgb(IMG *ppm, IMG *r, IMG *g, IMG *b);	// 提取 RGB 三个分量
int combine_rgb(IMG *ppm, IMG *r, IMG *g, IMG *b);	// 合成 RGB 三个分量


// ----------------------倍率---------------------------
// 双线性灰度插值, (old_h, old_w) 是原图坐标
inline UC calc(IMG *ppm, double old_h, double old_w)
{
	int b = ppm->byte;
	int h = (int)old_h;
	int w = (int)old_w;
	int k = ppm->width * ppm->byte;		// 一行的字节数

	if (h < 0 || w < 0 || h > ppm->height - 1 || w > ppm->width - 1)
		return NUM;		// 越界了返回 128 灰度值

	// 四点的坐标 (h, w), (h+1, w), (h, w+b), (h+1, w+b)
	double left = (1.0 - (old_h - h)) * (int)ppm->data[h * k + w] 
		+ (1.0 - (h + 1.0 - old_h)) * (int)ppm->data[(h + 1) * k + w];
	double right = (1.0 - (old_h - h)) * (int)ppm->data[h * k + w + b] 
		+ (1.0 - (h + 1.0 - old_h)) * (int)ppm->data[(h + 1) * k + w + b];
	return (UC)((1.0 - (old_w - w)) * left + (1.0 - (w + 1.0 - old_w)) * right);
/*
	return (UC)((1.0 - (old_w - w)) * ((1.0 - (old_h - h)) 
		* (int)ppm->data[h * k + w] + (1.0 - (h + 1.0 - old_h)) 
		* (int)ppm->data[(h + 1) * k + w]) + (1.0 - (w + 1.0 - old_w)) 
		* ((1.0 - (old_h - h)) * (int)ppm->data[h * k + w + b] 
		+ (1.0 - (h + 1.0 - old_h)) * (int)ppm->data[(h + 1) * k + w + b]));
*/
}

// 临近点插值, 靠得最近的点, 作为插值
inline UC calc2(IMG *ppm, double old_h, double old_w)
{
	int b = ppm->byte;
	int h = (int)old_h;
	int w = (int)old_w;
	int k = ppm->width * ppm->byte;		// 一行的字节数

	if (h < 0 || w < 0 || h > ppm->height - 1 || w > ppm->width - 1)
		return NUM;		// 越界了返回 128 灰度值

	if ((old_h - h) > (h + 1.0 - old_h))	// 离 h+1 的两点近
	{
		if ((old_w - w) > (w + 1.0 - old_w))	// 离 w+1 近
			return ppm->data[(h + 1) * k + w + b];
		else return ppm->data[(h + 1) * k + w];
	}
	else	// 离 h 的两点近
	{
		if ((old_w - w) > (w + 1.0 - old_w))	// 离 w+1 近
			return ppm->data[h * k + w + b];
		else return ppm->data[h * k + w];
	}
	return NUM;
}

// 灰度图的倍率
void change_size_gray(IMG *ppm, double h, double w)
{
	IMG tmp = *ppm;		// 临时图像空间
	int i, j;
	double old_h, old_w;

	// 倍率不为 1, 则变换
	if ((h-1) > EPS || (w-1) > EPS || (1-h) > EPS || (1-w) > EPS)
	{
		// 计算变换后大小, 分配新空间
		tmp.height = (int)(ppm->height * h);
		tmp.width = (int)(ppm->width * w);
		tmp.data = (UC *)malloc(tmp.height 
			* tmp.width * tmp.byte * sizeof(UC));
		if (tmp.data == NULL) return ;

		int ww = tmp.width * tmp.byte;		// 新宽度
		for (i=0; i<tmp.height; i++)
		{
			for (j=0; j<tmp.width; j++)
			{
				old_h = i * (1 / h);
				old_w = j * (1 / w);
				tmp.data[i * ww + j] = calc(ppm, old_h, old_w);		// 双线性插值
//				tmp.data[i * ww + j] = calc2(ppm, old_h, old_w);	// 最近邻插值
			}
		}
		copy_img(ppm, &tmp);		// 复制回 ppm 中
		close_img(&tmp);
	}
}

// 倍率, 横向变为 w0 倍, 纵向变为 h0 倍, 变换中心 (w0, h0)
void change_size(IMG *ppm, double h, double w)
{
	if (ppm->flag == '5')		// 灰度图直接处理
	{
		change_size_gray(ppm, h, w);
	}
	else if (ppm->flag == '6')	// 彩色图对三个分量分别处理, 再合成
	{
		IMG r, g, b;

		r.data = g.data = b.data = NULL;	// 初始化
		if (extract_rgb(ppm, &r, &g, &b))	// 提取 RGB 三个分量
		{
			// 这里可以用三条线程加快处理速度
			change_size_gray(&r, h, w);
			change_size_gray(&g, h, w);
			change_size_gray(&b, h, w);

			combine_rgb(ppm, &r, &g, &b);	// 合成原图

			close_img(&r);		// 释放内存
			close_img(&g);
			close_img(&b);
		}
	}
}


// ----------------旋转 a 角度, 旋转中心 (w0, h0)-----------------

// 根据原来的坐标, 计算出新坐标
inline void old_to_new(double h, double h0, double w0, 
					   double old_h, double old_w, 
					   double *new_h, double *new_w)
{
	*new_h = (old_w - w0) * sin(h) + (old_h - h0) * cos(h) + h0;
	*new_w = (old_w - w0) * cos(h) - (old_h - h0) * sin(h) + w0;
}

// 灰度图的旋转
// 先计算其四个顶点, 计算出新图片的大小, 然后平移到图片的中间来
void change_degree_gray(IMG *ppm, double a)
{
	IMG tmp = *ppm;				// 临时图像空间
	double hu = a * PI / 180;	// 弧度
	double new_h[4], new_w[4];	// 新图的四点坐标
	int h = ppm->height;
	int w = ppm->width;
	int h0 = ppm->height / 2;	// 旋转中心 (h0, w0)
	int w0 = ppm->width / 2;
	int i, j, ii, jj;

	// 计算变换后的四个顶点坐标
	old_to_new(hu, h0, w0, 0, 0, new_h + 0, new_w + 0);
	old_to_new(hu, h0, w0, 0, w-1, new_h + 1, new_w + 1);
	old_to_new(hu, h0, w0, h-1, w-1, new_h + 2, new_w + 2);
	old_to_new(hu, h0, w0, h-1, 0, new_h + 3, new_w + 3);
//	for (i=0; i<4; i++) printf("%lf, %lf\n", new_h[i], new_w[i]);

	// 计算变换后大小, 分配新空间
	double max_h, min_h, max_w, min_w;
	min_h = max_h = new_h[0];
	max_w = min_w = new_w[0];
	for (i=1; i<4; i++)			// 找出差值
	{
		if (new_h[i] < min_h) min_h = new_h[i];
		if (new_h[i] > max_h) max_h = new_h[i];
		if (new_w[i] < min_w) min_w = new_w[i];
		if (new_w[i] > max_w) max_w = new_w[i];
	}
	tmp.height = (int)(max_h - min_h + 1);		// 新高度 maxh - minh
	tmp.width = (int)(max_w - min_w + 1);
	tmp.data = (UC *)malloc(tmp.height * tmp.width * tmp.byte * sizeof(UC));
	if (tmp.data == NULL) return ;

	// 灰度插值
	int ww = tmp.width * tmp.byte;
	double old_h, old_w;
	// ii 是 [min_h, max_h] 的循环, 有负数
	// jj 是 [min_w, max_w] 的循环
	// i, j是新图像的循环
	for (i=0, ii=(int)min_h; i<tmp.height; i++, ii++)
	{
		for (j=0, jj=(int)min_w; j<tmp.width; j++, jj++)
		{
			old_h = (ii - h0) * cos(hu) - (jj - w0) * sin(hu) + h0;
			old_w = (jj - w0) * cos(hu) + (ii - h0) * sin(hu) + w0;
			tmp.data[i * ww + j] = calc(ppm, old_h, old_w);		// 双线性插值
//			tmp.data[i * ww + j] = calc2(ppm, old_h, old_w);	// 最近邻插值
		}
	}
	copy_img(ppm, &tmp);		// 复制回 ppm 中
	close_img(&tmp);
}

// 旋转 a 度 , a 在 [0, 360] 度之间
void change_degree(IMG *ppm, double a)
{
	if (ppm->flag == '5')		// 灰度图直接处理
	{
		change_degree_gray(ppm, a);
	}
	else if (ppm->flag == '6')	// 彩色图对三个分量分别处理, 再合成
	{
		IMG r, g, b;

		r.data = g.data = b.data = NULL;	// 初始化
		if (extract_rgb(ppm, &r, &g, &b))	// 提取 RGB 三个分量
		{
			// 这里可以用三条线程加快处理速度
			change_degree_gray(&r, a);
			change_degree_gray(&g, a);
			change_degree_gray(&b, a);

			combine_rgb(ppm, &r, &g, &b);	// 合成原图

			close_img(&r);		// 释放内存
			close_img(&g);
			close_img(&b);
		}
	}
}

int main(void)
{
	IMG ppm;
	char path[512];		// 路径
	double a, h, w;

	printf("输入图片路径: 或把图片直接拖到本窗口上\n");
	gets(path);

	if (open_img(&ppm, path))
	{
		printf("旋转 a 度[0, 360], h 倍高, w 倍宽, 输入a, h, w: ");
		scanf("%lf %lf %lf", &a, &h, &w);

		change_size(&ppm, h, w);	// 倍率
		change_degree(&ppm, a);		// 旋转

		save_img(&ppm, "result.ppm");
		close_img(&ppm);
		printf("处理完成!\n");
	}
	else printf("打开错误!\n");

	return 0;
}

// ------------------------------------------------------------
// ---------------- 以下代码不是本实验重点 --------------------
// ------------------------------------------------------------

// 提取 RGB 三个分量到 r, g, b, 如果 r 有数据会被释放掉
int extract_rgb(IMG *ppm, IMG *r, IMG *g, IMG *b)
{
	if (ppm == NULL) return 0;

	if (r->data != NULL) free(r);	// 释放原来的
	if (g->data != NULL) free(g);
	if (b->data != NULL) free(b);
	*r = *g = *b = *ppm;
	r->byte = g->byte = b->byte = 1;
	r->flag = g->flag = b->flag = '5';

	int new_size = r->height * r->width * r->byte * sizeof(UC);
	r->data = (UC *)malloc(new_size);	// 分配空间
	g->data = (UC *)malloc(new_size);
	b->data = (UC *)malloc(new_size);

	int i, j, top;
	int ww = ppm->width * ppm->byte;
	for (i=0, top=0; i<ppm->height; i++)	// 分离
	{
		for (j=0; j<ww; j+=3, top++)
		{
			r->data[top] = ppm->data[i * ww + j];
			g->data[top] = ppm->data[i * ww + j + 1];
			b->data[top] = ppm->data[i * ww + j + 2];
		}
	}
	return 1;
}

// 合成 RGB 三个分量, 需确保 r, g, b, 大小一样
int combine_rgb(IMG *ppm, IMG *r, IMG *g, IMG *b)
{
	if (r == NULL || g == NULL || b == NULL) return 0;
	if (ppm->data) free(ppm->data);		// 释放原来的

	ppm->type = 'P';
	ppm->flag = '6';
	ppm->maxv = 255;
	ppm->height = r->height;
	ppm->width = r->width;
	ppm->byte = 3;
	ppm->data = (UC *)malloc(ppm->height 
		* ppm->width * ppm->byte * sizeof(UC));

	int i, j, top;
	int ww = ppm->width * ppm->byte;
	for (i=0, top=0; i<ppm->height; i++)	// 合成
	{
		for (j=0; j<ww; j+=3, top++)
		{
			ppm->data[i * ww + j] = r->data[top];
			ppm->data[i * ww + j + 1] = g->data[top];
			ppm->data[i * ww + j + 2] = b->data[top];
		}
	}
	return 1;
}

// 读取一张图片, 只能读取灰度图和彩色图
int open_img(IMG *ppm, char *path)
{
	FILE *fp = fopen(path, "rb");
	int ret = 1;	// 返回值
	int w, h;
	int i, f;

	if (fp != NULL)
	{
		ppm->type = fgetc(fp);
		ppm->flag = fgetc(fp);
		fgetc(fp);		// 多余空格
		fscanf(fp, "%d", &(ppm->width));
		fgetc(fp);
		fscanf(fp, "%d", &(ppm->height));
		fgetc(fp);
		fscanf(fp, "%d", &(ppm->maxv));
		fgetc(fp);

		// 分配内存空间
		w = ppm->width;
		h = ppm->height;
		if (ppm->flag == '5')
		{
			ppm->data = (UC *)malloc(w * h * sizeof(UC));
			ppm->byte = 1;
		}
		else if (ppm->flag == '6')
		{
			ppm->data = (UC *)malloc(w * h * 3 * sizeof(UC));
			ppm->byte = 3;
		}
		else ret = 0;

		// 读取数据
		if (ret != 0 && ppm->data != NULL)
		{
			// 读取数据, fread() 一次只能读入 64KB 的数据, 这里分段读
			for (i=0, f=1; i<h && f!=0; i++)
			{
				f = fread(ppm->data + i * w * ppm->byte, 1, w * ppm->byte, fp);
			}
			if (f == 0)		// 一部分数据读取失败
			{
				free(ppm->data);
				ret = 0;
			}
		}
		else ret = 0;
		fclose(fp);
	}
	else ret = 0;

	return ret;
}

// 写入一张图片
int save_img(IMG *ppm, char *path)
{
	FILE *fp = fopen(path, "wb");
	int w = ppm->width;
	int h = ppm->height;
	int i, f;

	if (fp == NULL) return 0;

	// 写入 ppm 头
	fprintf(fp, "%c%c\n%d\n%d\n%d\n", 
		ppm->type, ppm->flag, w, h, ppm->maxv);

	// 写入图像数据
	for (i=0, f=1; i<h && f!=0; i++)
	{
		f = fwrite(ppm->data + i * w * ppm->byte, 1, w * ppm->byte, fp);
	}
	if (f == 0) return 0;	// 写入错误
	fclose(fp);

	return 1;
}

int copy_img(IMG *d, IMG const *s)		// 复制
{
	if (d == NULL || s == NULL) return 0;

	if (d->data != NULL) free(d->data);		// 释放原来的图像
	*d = *s;								// 复制成员
	int size = s->height * s->width * s->byte;
	d->data = (UC *)malloc(size * sizeof(UC));

	for (int i=0; i<size; i++)
		d->data[i] = s->data[i];	// 复制图像数据
	return 0;
}

// 关闭一张图片
int close_img(IMG *ppm)
{
	if (ppm->data) free(ppm->data);
	memset(ppm, 0, sizeof(IMG));

	return 0;
}
