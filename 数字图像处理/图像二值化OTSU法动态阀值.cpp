
// 图像二值化 ostu 算法自动确定阀值
// 原理见 OTSU 确定阀值报告

#include <stdio.h>
#include <string.h>
#include <malloc.h>

typedef unsigned char UC;
typedef unsigned long int ULI;

typedef struct		// ppm 图像
{
	char type;		// P 标志
	char flag;		// 几号
	int  width;		// 宽
	int  height;	// 长
	int  maxv;		// 最大量化值
	int  byte;		// 每个像素占用字节数
	UC   *data;		// 图像数据
}IMG;

int open_img(IMG *ppm, char *path);		// 打开
int save_img(IMG *ppm, char *path);		// 保存
int close_img(IMG *ppm);				// 关闭

// 彩色图转灰度图
// 转换公式 : y = 0.299R + 0.587G + 0.114B
void change_gray(IMG *ppm)
{
	int i, j;
	int ww = ppm->width * ppm->byte;
	UC *tmp = (UC *)malloc(ppm->height * ppm->width * sizeof(UC));
	int cur;

	for (i=0, cur=0; i<ppm->height; i++)
		for (j=0; j<ww; j+=3)
		{
			tmp[cur++] = (UC)(0.299 * ppm->data[i * ww + j]
				+ 0.587 * ppm->data[i * ww + j + 1]
				+ 0.114 * ppm->data[i * ww + j + 2]);
		}
	free(ppm->data);	// 释放原来的
	ppm->flag = '5';
	ppm->byte = 1;
	ppm->data = tmp;	// 放入新的
}

// 对图像 ppm 按照阀值 t 进行二值化
void threshold(IMG *ppm, UC t)
{
	int i;
	int size = ppm->height * ppm->width * ppm->byte;

	for (i=0; i<size; i++)
	{
		if (ppm->data[i] > t) ppm->data[i] = ppm->maxv;
		else ppm->data[i] = 0;
	}
}

// otsu 算法根据图片计算出阀值, 返回最佳阀值
UC otsu_threshold(IMG *ppm)
{
	if (ppm->flag != '5') return 0;		// 确保是灰度图

	int size = ppm->height * ppm->width * ppm->byte;
	ULI *count = (ULI *)malloc((ppm->maxv + 2) * sizeof(ULI));
	UC T = 0;
	int i, t;
	double p1, m1, n1, c1, p2, m2, n2, c2;
	// p1 背景概率, m1 背景平均灰度, n1 背景总灰度, c1 背景总个数
	double sum, mg, d, maxd;
	// sum 全图灰度总值, mg 全图平均灰度, d 间类方差, maxd 最大间类方差

	maxd = -1000000;		// 初始化
	c1 = c2 = n1 = n2 = p1 = p2 = m1 = m2 = sum = mg = d = 0;
	for (i=ppm->maxv; i>=0; i--) count[i] = 0;

	for (i=0; i<size; i++) count[ppm->data[i]]++;	// 统计直方图
	for (i=ppm->maxv; i>=0; i--) sum = sum + (count[i] * i);
	mg = sum / size;				// mg 计算全图平均灰度

	for (t=0; t<ppm->maxv; t++)		// t[0, 254]
	{
		c1 = c1 + count[t];			// 像素个数
		c2 = size - c1;
		n1 = n1 + (count[t] * t);	// 灰度值总数
		n2 = sum - n1;
		if (n1 > 0 && n2 > 0)
		{
			m1 = n1 / c1;		// 平均灰度
			m2 = n2 / c2;
			p1 = c1 / size;		// 背景概率
			p2 = c2 / size;

			// 计算间类方差
			d = p1 * (m1 - mg) * (m1 - mg) 
				+ p2 * (m2 - mg) * (m2 - mg);
//			d = p1 * p2 * (m1 - m2) * (m1 - m2);	// 公式2
			if (d > maxd)
			{
				maxd = d;
				T = t;		// 记录最佳分割值
			}
		}
	}
	free(count);
	return T;
}

// otsu 算法二值化自动确定阀值
void otsu(IMG *ppm)
{
	// 彩色图先转灰度图
	if (ppm->flag == '6') change_gray(ppm);

	int t = otsu_threshold(ppm);	// otsu 计算阀值
	printf("最佳阀值: %d\n", t);
	threshold(ppm, t);				// 二值化处理
}

int main(void)
{
	IMG ppm;
	char path[512];		// 路径

	printf("输入图片路径: 或把图片直接拖到本窗口上\n");
	scanf("%s", path);

	if (open_img(&ppm, path))
	{
		otsu(&ppm);
		save_img(&ppm, "二值化后.ppm");
		close_img(&ppm);
	}
	return 0;
}

// ------------------------------------------------------------
// ---------------- 以下代码不是本实验重点 --------------------
// ------------------------------------------------------------

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

// 关闭一张图片
int close_img(IMG *ppm)
{
	if (ppm->data) free(ppm->data);
	memset(ppm, 0, sizeof(IMG));

	return 0;
}
