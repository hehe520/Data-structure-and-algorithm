
// 伪彩色处理
// 编译环境 VC++6.0, C++编译

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <math.h>

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

UC LUT_R[256];		// RGB 的映射函数
UC LUT_G[256];
UC LUT_B[256];

int open_img(IMG *ppm, char *path);		// 打开
int save_img(IMG *ppm, char *path);		// 保存
int copy_img(IMG *d, IMG const *s);		// 复制
int close_img(IMG *ppm);				// 关闭
int combine_rgb(IMG *ppm, IMG *r, IMG *g, IMG *b);	// 合成 RGB 三个分量

// 灰度映射
void change_gray(IMG *ppm, UC *LUT)
{
	int size = ppm->height * ppm->width * ppm->byte;
	int i;

	for (i=0; i<size; i++) ppm->data[i] = LUT[ppm->data[i]];
}

// 伪彩色处理
int change_color(IMG *ppm)
{
	IMG rgb[3];		// RGB 三个分量
	int i;

	if (ppm == NULL || ppm->data == NULL || ppm->flag != '5')
		return 0;

	// 把 ppm 复制三份, 也可以复制一份处理一份, 节省一些空间
	for (i=0; i<3; i++) rgb[i].data = NULL;
	for (i=0; i<3; i++) copy_img(rgb + i, ppm);
	
	change_gray(rgb + 0, LUT_R);	// 分别灰度映射
	change_gray(rgb + 1, LUT_G);
	change_gray(rgb + 2, LUT_B);

	combine_rgb(ppm, rgb + 0, rgb + 1, rgb + 2);	// 合成彩色图

	return 1;
}

void init_LUT(void)		// 初始化映射函数
{
	int i;

	for (i=0; i<256; i++) 
	{
		LUT_R[i] = (UC)i;			// 红分量
		LUT_B[i] = (UC)(255 - i);	// 蓝分量
	}
	for (i=0; i<128; i++) LUT_G[i] = (UC)(2 * i);	// 绿分量
	for (i=128; i<256; i++) LUT_G[i] = (UC)(2 * (255 - i));

	for (i=0; i<256; i++)
		printf("%u\t%u\t%u\n", LUT_R[i], LUT_G[i], LUT_B[i]);
}

int main(void)
{
	IMG ppm;
	char path[512];		// 路径

	printf("输入图片路径: 或把图片直接拖到本窗口上\n");
	gets(path);

	if (open_img(&ppm, path))
	{
		init_LUT();		// 初始化映射函数
		if (change_color(&ppm))
		{
			save_img(&ppm, "伪彩色变换.ppm");
			printf("处理完成!\n");
		}
		else printf("已经是彩色图了!\n");
		close_img(&ppm);
	}
	else printf("打开错误!\n");

	return 0;
}

// ------------------------------------------------------------
// ---------------- 以下代码不是本实验重点 --------------------
// ------------------------------------------------------------

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
