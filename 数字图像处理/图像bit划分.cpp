
// 一个像素抽出其每个位比特, 组成新的图像

#include <stdio.h>
#include <string.h>
#include <malloc.h>

typedef unsigned char UC;

typedef struct		// ppm 图像
{
	char type;		// P 标志
	char flag;		// 几号
	int  width;		// 宽
	int  height;	// 长
	int  maxv;		// 最大量化值
	int  byte;		// 一个像素占的字节数
	UC   *data;		// 图像数据
}img;

int open_img(img *ppm, char *path);		// 打开
int save_img(img *ppm, char *path);		// 保存
int copy_img(img *d, img *s);			// 复制图片
void close_img(img *ppm);				// 关闭

void output_8bit(img *ppm)		// 输出八张图片
{
	int size = ppm->height * ppm->width * ppm->byte;
	unsigned char b;	// 掩码
	img tmp;
	int i, j;
	char path[50];

	for (i=0, b=0x80; i<8; i++, b>>=1)
	{
		copy_img(&tmp, ppm);	// 复制一份新的
		for (j=0; j<size; j++)
			tmp.data[j] &= b;	// 其它比特置零
		sprintf(path, "%d.ppm", i);
		save_img(&tmp, path);	// 保存
		close_img(&tmp);
	}
}

int main(void)
{
	img ppm;
	char path[256];

	printf("输入图片路径: ");
	scanf("%s", path);

	if (open_img(&ppm, path))
	{
		output_8bit(&ppm);
		close_img(&ppm);
	}
	else printf("打开图片错误\n");

	return 0;
}

int copy_img(img *d, img *s)	// 复制图片
{
	int size = s->height * s->width * s->byte * sizeof(UC);

	if (d == NULL || s == NULL) return 0;

	*d = *s;
	d->data = (UC *)malloc(size);		// 申请空间
	if (d->data == NULL) return 0;

	memcpy(d->data, s->data, size);		// 复制图像

	return 1;
}

// 读取一张图片, 只能读取灰度图和彩色图
int open_img(img *ppm, char *path)
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
		if (ppm->type == 'P')
		{
			if (ppm->flag == '2' || ppm->flag == '5')
			{
				// 灰度图像, 一个字节
				ppm->data = (UC *)malloc(w * h * sizeof(UC));
				ppm->byte = 1;
			}
			else if (ppm->flag == '3' || ppm->flag == '6')
			{	
				// 彩色图像, 三个字节
				ppm->data = (UC *)malloc(w * h * 3 * sizeof(UC));
				ppm->byte = 3;
			}
			else ret = 0;
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

// 写入一张图片
int save_img(img *ppm, char *path)
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

	return 1;
}

// 关闭一张图片
void close_img(img *ppm)
{
	if (ppm->data) free(ppm->data);
	memset(ppm, 0, sizeof(img));
}
