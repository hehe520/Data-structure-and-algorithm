
// 图像的几何变换
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
int extract_rgb(IMG *ppm, IMG *r, IMG *g, IMG *b);		// 提取 RGB 三个分量
int combine_rgb(IMG *ppm, IMG *r, IMG *g, IMG *b);		// 合成 RGB 三个分量
inline UC calc(IMG *ppm, double old_h, double old_w);	// 双线性插值
inline UC calc2(IMG *ppm, double old_h, double old_w);	// 最近邻插值


// ----------------------- 图像的变换1 ----------------------------
void change1_gray(IMG *ppm, int m)
{
	IMG tmp = *ppm;		// 临时图像空间
	int i, j;
	double old_h, old_w;

	// 计算变换后大小, 分配新空间
	tmp.data = (UC *)malloc(tmp.height 
		* tmp.width * tmp.byte * sizeof(UC));
	if (tmp.data == NULL) return ;
	
	int ww = tmp.width * tmp.byte;		// 新宽度
	double k = (double)m / ppm->height;
	for (i=0; i<tmp.height; i++)
	{
		for (j=0; j<tmp.width; j++)
		{
			old_h = i;		// 公式见报告
			old_w = (j-i*k) / (1 - i / (double)ppm->height);
			tmp.data[i * ww + j] = calc(ppm, old_h, old_w);		// 双线性插值
//			tmp.data[i * ww + j] = calc2(ppm, old_h, old_w);	// 最近邻插值
		}
	}
	copy_img(ppm, &tmp);		// 复制回 ppm 中
	close_img(&tmp);
}

// 倍率, 横向变为 w0 倍, 纵向变为 h0 倍, 变换中心 (w0, h0)
void change1(IMG *ppm, int m)
{
	if (ppm->flag == '5')		// 灰度图直接处理
	{
		change1_gray(ppm, m);
	}
	else if (ppm->flag == '6')	// 彩色图对三个分量分别处理, 再合成
	{
		IMG r, g, b;
		r.data = g.data = b.data = NULL;	// 初始化
		if (extract_rgb(ppm, &r, &g, &b))	// 提取 RGB 三个分量
		{
			change1_gray(&r, m);
			change1_gray(&g, m);
			change1_gray(&b, m);
			combine_rgb(ppm, &r, &g, &b);	// 合成原图
			close_img(&r);		// 释放内存
			close_img(&g);
			close_img(&b);
		}
	}
}

// ----------------------- 图像的变换2 ----------------------------
void change2_gray(IMG *ppm, int A, int T, int B)
{
	IMG tmp = *ppm;		// 临时图像空间
	int i, j;
	double old_h, old_w;

	// 计算变换后大小, 分配新空间
	tmp.height += (2 * A);
	tmp.data = (UC *)malloc(tmp.height 
		* tmp.width * tmp.byte * sizeof(UC));
	if (tmp.data == NULL) return ;
	
	int ww = tmp.width * tmp.byte;		// 新宽度
	for (i=0; i<tmp.height; i++)
	{
		for (j=0; j<tmp.width; j++)
		{
			old_h = (i-A) - A * sin(((2.0 * T * PI) / tmp.width) * j + B);
			old_w = j;		// 公式见报告
			tmp.data[i * ww + j] = calc(ppm, old_h, old_w);		// 双线性插值
//			tmp.data[i * ww + j] = calc2(ppm, old_h, old_w);	// 最近邻插值
		}
	}
	copy_img(ppm, &tmp);		// 复制回 ppm 中
	close_img(&tmp);
}

// 倍率, 横向变为 w0 倍, 纵向变为 h0 倍, 变换中心 (w0, h0)
void change2(IMG *ppm, int A, int T, int B)
{
	if (ppm->flag == '5')		// 灰度图直接处理
	{
		change2_gray(ppm, A, T, B);
	}
	else if (ppm->flag == '6')	// 彩色图对三个分量分别处理, 再合成
	{
		IMG r, g, b;
		r.data = g.data = b.data = NULL;	// 初始化
		if (extract_rgb(ppm, &r, &g, &b))	// 提取 RGB 三个分量
		{
			change2_gray(&r, A, T, B);
			change2_gray(&g, A, T, B);
			change2_gray(&b, A, T, B);
			combine_rgb(ppm, &r, &g, &b);	// 合成原图
			close_img(&r);		// 释放内存
			close_img(&g);
			close_img(&b);
		}
	}
}

int main(void)
{
	IMG ppm, tmp;
	char path[512];		// 路径
	int m, A, T, B;

	printf("输入图片路径: 或把图片直接拖到本窗口上\n");
	gets(path);

	if (open_img(&ppm, path))
	{
		tmp.data = NULL;

		// 选做实验一:
		printf("1. 请输入 0 到 %d 的一个系数: ", ppm.width);
		scanf("%d", &m);

		copy_img(&tmp, &ppm);
		change1(&tmp, m);		// 变换1
		save_img(&tmp, "变换1.ppm");

		// 选做实验二: 
		printf("2. 请输入振幅 A, 周期数 T, 初相 B, 请输入 A T B : ");
		scanf("%d %d %d", &A, &T, &B);

		copy_img(&tmp, &ppm);
		change2(&tmp, A, T, B);		// 变换2
		save_img(&tmp, "变换2.ppm");
		
/*		for (int i=0, T=1; i<3; i++, T+=T)
		{
			for (A=5, B=0; A<21; A+=5)
			{
				copy_img(&tmp, &ppm);
				change2(&tmp, A, T, B);		// 变换2
				sprintf(path, "%d_%d_%d.ppm", T, A, B);
				save_img(&tmp, path);
			}
		}*/
		close_img(&ppm);
		close_img(&tmp);
		printf("处理完成!\n");
	}
	else printf("打开错误!\n");

	return 0;
}

// ------------------------------------------------------------
// ---------------- 以下代码不是本实验重点 --------------------
// ------------------------------------------------------------

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
