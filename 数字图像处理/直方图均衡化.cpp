
// 灰度图直接均衡化, 彩色图对R, G, B 三个分量分别均衡化

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
	int  width;		// 宽
	int  height;	// 长
	int  maxv;		// 最大量化值
	int  byte;		// 每个像素占用字节数
	UC   *data;		// 图像数据
}IMG;

int open_img(IMG *ppm, char *path);		// 打开
int save_img(IMG *ppm, char *path);		// 保存
int close_img(IMG *ppm);				// 关闭

// 求归一化灰度直方图
int hist_1(IMG *img, double *count)
{
	int size = img->height * img->width * img->byte;
	int i;

	// 统计像素个数, 及归一化
	for (i=img->maxv; i>=0; i--) count[i] = 0.0;
	for (i=0; i<size; i++) count[img->data[i]] += 1;
	for (i=img->maxv; i>=0; i--) count[i] /= size;

	return 1;
}

// 直方图均衡化, 只支持灰度图
// 1. 先计算归一化直方图, 如果某一个灰度的像素很多, 那么其概论值越大
// 2. 计算累积直方图, 如果某一个灰度的概率值越大, 那么累积后曲线上升的快
// 即变化率越大, 所以灰度映射后这块的灰度值就拉大了
// 计算映射函数, 然后灰度变换
void hist(IMG *ppm)
{
	int size = ppm->height * ppm->width * ppm->byte;
	double *count;		// 归一化直方图
	int i;

	count = (double *)malloc((ppm->maxv + 2) * sizeof(double));
	hist_1(ppm, count);				// 归一化直方图
	for (i=1; i<=ppm->maxv; i++) 
		count[i] += count[i-1];		// 累积直方图
	for (i=0; i<=ppm->maxv; i++) 
		count[i] = count[i] * (ppm->maxv + 1) - 1;	// 计算映射函数
	for (i=0; i<=ppm->maxv; i++)		// 控制范围
	{
		if (count[i] < 0) count[i] = 0;
		else if (count[i] > 255) count[i] = 255;
	}
	for (i=0; i<size; i++)			 	// 对原图映射
		ppm->data[i] = (UC)count[ppm->data[i]];
	free(count);
}

// 直方图均衡化, 支持灰度图与彩色图
int hist_EQ(IMG *ppm)
{
	int i, j, k;

	if (ppm->flag == '5')	// 灰度图直接处理
	{
		hist(ppm);
		return 1;
	}
	else if (ppm->flag == '6')	// 彩色图三个分量分别处理
	{
		int ww = ppm->width * ppm->byte;
		int cur;
		IMG tmp;
		tmp = *ppm;
		tmp.byte = 1;
		tmp.flag = '5';
		tmp.data = (UC *)malloc(tmp.width * tmp.height * sizeof(UC));
		for (k=0; k<3; k++)
		{
			// 一个分量复制到 tmp
			for (i=0, cur=0; i<ppm->height; i++)
				for (j=0; j<ww; j+=ppm->byte)
					tmp.data[cur++] = ppm->data[i * ww + j + k];
			hist(&tmp);		// 均衡化

			// 覆盖到原图像
			for (i=0, cur=0; i<ppm->height; i++)
				for (j=0; j<ww; j+=ppm->byte)
					ppm->data[i * ww + j + k] = tmp.data[cur++];
		}
		close_img(&tmp);
		return 1;
	}
	return 0;
}

// 计算图片平均信息量
float E(IMG *ppm)
{
	double *count = (double *)malloc((ppm->maxv + 2) * sizeof(double));
	double avg = 0;
	int i;

	hist_1(ppm, count);		// 归一化直方图
	for (i=0; i<=ppm->maxv; i++)
	{
		if (count[i] > 0.000000001)
			avg = avg + (-count[i] * log(count[i]));
	}
	free(count);

	return (float)avg;
}

int main(void)
{
	IMG ppm;
	char path[512];		// 路径

	printf("输入图片路径: 或把图片直接拖到本窗口上\n");
	scanf("%s", path);

	if (open_img(&ppm, path))
	{
		printf("均衡化前平均信息量: %f\n", E(&ppm));
		hist_EQ(&ppm);
		printf("均衡化后平均信息量: %f\n", E(&ppm));
		save_img(&ppm, "均衡化后.ppm");
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
