
// 输入一张图片, 输出其三个通道的图片

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef unsigned char UC;

typedef struct		// ppm 彩色像素点, 三个字节
{
	UC r;
	UC g;
	UC b;
}ppm_RGB;

// 动态分配一个二维数组
ppm_RGB **new_array(int x, int y)
{
	ppm_RGB **tmp = NULL;

	if (x <= 0 || y <= 0) return NULL;

	tmp = (ppm_RGB **)malloc(x * sizeof(ppm_RGB *));
	for (int i=0; i<x; i++)
		tmp[i] = (ppm_RGB *)malloc(y * sizeof(ppm_RGB));
	return tmp;
}

// 释放一个二维数组
void del_array(ppm_RGB **tmp, int x)
{
	for (int i=0; i<x; i++)
		free(tmp[i]);
	free(tmp);
	tmp = NULL;
}

// 将 tmp 图像写到 path 中
void write_pic(UC *tmp, char *type, 
			   int h, int w, int maxv, char *path)
{
	FILE *fp = fopen(path, "wb");
	int i, size;

	if (fp == NULL) return ;

	// 文件头
	fprintf(fp, "%s\n%d %d\n%d\n", type, w, h, maxv);
	for (i=0, size = w * sizeof(UC); i<h; i++)
	{
		fwrite(tmp + i * w, 1, size, fp);
	}
	fclose(fp);
}

int main(void)
{
	FILE *fp = fopen("test.ppm", "rb");		// 需要一张 test.ppm 图像
	ppm_RGB **data = NULL;		// 原图像
	UC *tmp = NULL;			// RGB的灰度图
	int width, height;
	int maxv;
	char str[31];
	int i, j;

	if (fp != NULL)
	{
		fscanf(fp, "%s", str);
		if (strcmp(str, "P6") == 0)
		{
			fscanf(fp, "%d", &width);	// 读取宽
			fscanf(fp, "%d", &height);	// 读取高
			fscanf(fp, "%d", &maxv);	// 读取级数
			data = new_array(height, width);
			if (data != NULL)
			{
				int flag = 1;			// 读取字节数
				for (i=0; i<height && flag; i++)
				{
					// 因为 fread() 一次只能读取 64KB, 所以这里一行一行读
					flag = fread(data[i], 1, width * sizeof(ppm_RGB), fp);
				}
				// 读取完毕
				tmp = (UC *)malloc(width * height * sizeof(UC));
				int cur;
				if (tmp != NULL)
				{
					for (i=0, cur=0; i<height; i++)
						for (j=0; j<width; j++, cur++)
							*(tmp + cur) = data[i][j].r;	// 提取红色
					write_pic(tmp, "P5", height, width, maxv, "红色灰度图.ppm");

					for (i=0, cur=0; i<height; i++)
						for (j=0; j<width; j++, cur++)
							*(tmp + cur) = data[i][j].g;
					write_pic(tmp, "P5", height, width, maxv, "绿色灰度图.ppm");

					for (i=0, cur=0; i<height; i++)
						for (j=0; j<width; j++, cur++)
							*(tmp + cur) = data[i][j].b;
					write_pic(tmp, "P5", height, width, maxv, "蓝色灰度图.ppm");

					free(tmp);
				}
				else printf("内存分配失败!\n");
				del_array(data, height);
			}
			else printf("内存分配失败!\n");
		}
		else printf("不是P6彩色图像, %s\n", str);
		fclose(fp);
	}
	else printf("找不到 test.ppm\n");
	return 0;
}