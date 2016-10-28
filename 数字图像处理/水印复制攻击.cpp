
// 编译环境 VC++6.0, C++编译

#include <stdio.h>
#include <string.h>
#include <malloc.h>

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

int open_img(IMG *ppm, char *path);		// 打开, 支持 p4, p5, p6 图
int save_img(IMG *ppm, char *path);		// 保存, 支持 p4, p5, p6 图
int copy_img(IMG *d, IMG const *s);		// 复制, 支持 p4, p5, p6 图
int close_img(IMG *ppm);				// 关闭
int extract_bit(IMG *d, IMG const *s, int bit);		// 提取比特
int update_bit(IMG *d, IMG const *s, int bit);		// 合成比特

int main(void)
{
	IMG ppm, tmp, ppm2;
	char path[512];		// 路径
	int i;

	printf("输入有水印图片路径: 或把图片直接拖到本窗口上\n");
	gets(path);

	tmp.data = ppm.data = ppm2.data = NULL;		// 初始化
	if (open_img(&ppm, path))
	{
		printf("输入无水印的图片: \n");
		gets(path);
		if (open_img(&ppm2, path))
		{
			for (i=0; i<3; i++)		// 把前 3 个比特加到无水印的图片上
			{
				extract_bit(&tmp, &ppm, i);
				update_bit(&ppm2, &tmp, i);
			}
			save_img(&ppm2, "无水印的lena处理后.ppm");
			close_img(&ppm2);
			printf("处理完成!\n");
		}
		close_img(&ppm);
		close_img(&tmp);
	}
	else printf("打开错误!\n");

	return 0;
}

// ------------------------------------------------------------
// ---------------- 以下代码不是本实验重点 --------------------
// ------------------------------------------------------------

// d 是结果图像(二值), s 是灰度图, 提取第 bit 个比特
int extract_bit(IMG *d, IMG const *s, int bit)		// 提取比特
{
	UC b = 1;		// 提取位的模版
	UC tmp;
	int i, j, size;
	int top = 0;	// s 图像的下标

	if (d == NULL || s == NULL || s->data == NULL 
		|| bit < 0 || bit > 7) return 0;

	if (d->data) free(d->data);		// 释放原来的图像
	*d = *s;
	d->flag = '4';
	d->byte = 8;
	d->maxv = 1;
	d->data = (UC *)malloc(d->height * (d->width / d->byte) * sizeof(UC));

	b <<= bit;		// 提取模版
	size = d->height * d->width / d->byte;
	for (i=0; i<size; i++)
	{
		for (j=0, tmp=0; j<8; j++)
		{
			tmp <<= 1;		// tmp 一次放八个像素的比特
			if ((s->data[top++] & b) != 0) tmp |= 1;
		}
		d->data[i] = tmp;
	}
	return 1;
}

// 将 d 像素的第 i 个比特修改为 s , 要求 s 是二值图像, 大小和 d 相等
int update_bit(IMG *d, IMG const *s, int bit)		// 合成比特
{
	if (d == NULL || s == NULL || s->data == NULL
		|| d->height != s->height 
		|| d->width != s->width
		|| s->flag != '4') return 0;

	int i, size;
	int top;		// s 的下标
	UC j = 0x80;	// 最右边置 1
	UC b1 = 1;		// 或后为 1
	UC b0;			// 与后为 0

	b1 <<= bit;		// 提取位的模版 0000 0001
	b0 = ~b1;		// 取反后       1111 1110
	size = d->height * d->width * d->byte;
	for (top=i=0, j=0x80; i<size; i++)
	{
		if (s->data[top] & j)
			d->data[i] |= b1;
		else 
			d->data[i] &= b0;
		// j++
		j >>= 1;
		if (j == 0)		// 八个比特循环完了
		{
			j = 0x80;
			top++;
		}
	}
	return 1;
}

// 读取一张图片, 只能读取灰度图和彩色图
int open_img(IMG *ppm, char *path)
{
	FILE *fp = fopen(path, "rb");
	int ret = 1;	// 返回值
	int f = 1;
	int w, h, i;

	if (fp != NULL)
	{
		ppm->type = fgetc(fp);
		ppm->flag = fgetc(fp);
		fgetc(fp);		// 多余空格
		fscanf(fp, "%d", &(ppm->width));
		fgetc(fp);
		fscanf(fp, "%d", &(ppm->height));
		fgetc(fp);
		if (ppm->flag == '5' || ppm->flag == '6')
		{
			fscanf(fp, "%d", &(ppm->maxv));
			fgetc(fp);
		}
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
		else if (ppm->flag == '4')
		{
			ppm->data = (UC *)malloc(h * (w / 8) * sizeof(UC));
			ppm->byte = 8;
		}
		else ret = 0;

		// 读取数据
		if (ret != 0 && ppm->data != NULL)
		{
			if (ppm->flag == '5' || ppm->flag == '6')
			{
				// 读取数据, fread() 一次只能读入 64KB 的数据, 这里分段读
				for (i=0, f=1; i<h && f!=0; i++)
					f = fread(ppm->data + i * w * ppm->byte, 1, w * ppm->byte, fp);
			}
			else if (ppm->flag == '4')
			{
				int ww = ppm->width / ppm->byte;
				for (i=0, f=1; i<h && f!=0; i++)
					f = fread(ppm->data + i * ww, 1, ww, fp);
			}
			else ret = 0;

			if (f == 0)		// 一部分数据读取失败
			{
				free(ppm->data);
				ret = 0;
			}
		}
		else ret = 0;
		fclose(fp);		// 关闭文件
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
	int ret = 1;

	if (fp == NULL) return 0;

	if (ppm->flag == '5' || ppm->flag == '6')
	{
		// 写入 ppm 头
		fprintf(fp, "%c%c\n%d\n%d\n%d\n", 
			ppm->type, ppm->flag, w, h, ppm->maxv);

		// 写入图像数据
		for (i=0, f=1; i<h && f!=0; i++)
			f = fwrite(ppm->data + i * w * ppm->byte, 1, w * ppm->byte, fp);

		if (f == 0) ret = 0;	// 写入错误
	}
	else if (ppm->flag == '4')
	{
		// 写入 ppm 头
		fprintf(fp, "%c%c\n%d\n%d\n", 
			ppm->type, ppm->flag, w, h);

		// 写入图像数据
		int ww = w / ppm->byte;
		for (i=0, f=1; i<h && f!=0; i++)
			f = fwrite(ppm->data + i * ww, 1, ww, fp);

		if (f == 0) ret = 0;	// 写入错误
	}
	else ret = 0;
	fclose(fp);		// 记得关闭

	return ret;
}

int copy_img(IMG *d, IMG const *s)		// 复制
{
	if (d == NULL || s == NULL) return 0;

	if (d->data != NULL) free(d->data);		// 释放原来的图像
	*d = *s;								// 复制成员
	int size = s->height * s->width;

	if (s->flag == '5' || s->flag == '6')
		size *= s->byte;
	else if (s->flag == '4')
		size /= s->byte;

	d->data = (UC *)malloc(size * sizeof(UC));

	// 复制图像数据
	for (int i=0; i<size; i++) d->data[i] = s->data[i];

	return 0;
}

// 关闭一张图片
int close_img(IMG *ppm)
{
	if (ppm->data) free(ppm->data);
	memset(ppm, 0, sizeof(IMG));

	return 0;
}
