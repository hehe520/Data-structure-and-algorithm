
// 求灰度直方图, 输入一张图片, 输出直方图和CSV文件

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

// 函数声明
int open_img(IMG *ppm, char *path);		// 打开图片
void close_img(IMG *ppm);				// 关闭图片
void output_csv(ULI *count, int len);	// 输出到 CSV
void output_ppm(ULI *count, int len);	// 输出到图片

// 求灰度直方图
int imgHist(IMG *img, ULI *count, int *len)
{
	int size = img->height * img->width * img->byte;
	int i;

	for (i=img->maxv + 1; i>=0; i--) count[i] = 0;
	*len = img->maxv;

	// 统计像素个数
	for (i=0; i<size; i++)
	{
		count[img->data[i]]++;
	}
	return 1;
}

int main(void)
{
	IMG ppm;			// 图像
	char path[256];		// 路径
	ULI *count = NULL;	// 保存直方图数据
	int lenth;			// count 的长度

	printf("输入图片路径: 或者把图片直接拖到这个窗口上\n");
	scanf("%s", path);

	// 本实验只能打开 P5, P6 的 ppm 图
	if (open_img(&ppm, path))
	{
		count = (ULI *)malloc((ppm.maxv + 5) * sizeof(ULI));
		if (count != NULL)
		{
			imgHist(&ppm, count, &lenth);	// 求直方图
			output_csv(count, lenth);		// 输出 CSV 文件
			output_ppm(count, lenth);		// 输出直方图
			free(count);
		}
		close_img(&ppm);
	}
	else printf("打开图片错误\n");

	return 0;
}

// ---------------------------------------------------------
// -------------- 以下代码不是本实验重点 -------------------
// ---------------------------------------------------------

// 输出一张 P4 的直方图
void output_ppm(ULI *count, int len)
{
	int MAXN(ULI *count, int len);		// 数组最大元素
	ULI num = MAXN(count, len) + 10;	// 直方图最高的值
	const int h = 150;					// 高度
	const int w = len / 8 + 1;			// 宽度
	unsigned char binary;				// 掩码
	int i, j, k;
	FILE *fp = fopen("直方图.ppm", "wb");
	UC *data = NULL;

	if (fp != NULL)
	{
		data = (UC *)malloc(h * w * sizeof(UC));
		if (data != NULL)
		{
			memset(data, 0, h * w * sizeof(UC));
			// 初始化
			for (i=0; i<h; i++)
				for (j=0; j<w; j++)
					data[i * w + j] = 0;	// 设置白色

			// 画直方图
			k = 0;				// 宽度
			binary = 0x80;		// 二进制最右边置 1
			for (i=0; i<len; i++)		// 遍历 count
			{
				// 将 [0..maxn] 的像素个数压缩到 [0..150]
				for (j=h - (int)(count[i] * h / num); j<h; j++)
				{
					data[j * w + k] |= binary;	// 置 1, 变黑色
				}
				binary >>= 1;
				if (binary == 0) 
				{
					binary = 0x80;		// 重新置 1
					k++;
				}
			}
			// 保存到文件
			fprintf(fp, "%s\n%d\n%d\n", "P4", w * 8, h);
			for (i=0; i<h; i++)
				fwrite(data + i * w, 1, w, fp);
			free(data);
		}
		fclose(fp);
	}
}

int MAXN(ULI *count, int len)	// 返回数组中的最大值
{
	int i;
	ULI max = 0;

	for (i=0; i<len; i++)
		if (count[i] > max) max = count[i];
	return (int)max;
}

void output_csv(ULI *count, int len)		// 输出到 CSV
{
	FILE *fp = fopen("直方图.csv", "wb");
	int i;

	if (fp == NULL) return ;

	for (i=0; i<len; i++)
		fprintf(fp, "%d,%ld\n", i, count[i]);

	fclose(fp);
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
		if (ppm->flag == '5')	// 灰度图像, 一个字节
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
				return 0;
			}
		}
		else ret = 0;
		fclose(fp);
	}
	else ret = 0;

	return ret;
}

// 关闭一张图片
void close_img(IMG *ppm)
{
	if (ppm->data) free(ppm->data);
	memset(ppm, 0, sizeof(IMG));
}
