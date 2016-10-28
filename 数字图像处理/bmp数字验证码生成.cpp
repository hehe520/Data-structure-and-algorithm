
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pshpack2.h>
#include <windows.h>

typedef long int LONG;
typedef unsigned char BYTE;
typedef unsigned long int DWORD;
typedef unsigned short int WORD;

/*
// BMP 文件结构 
// 位图文件头 (bitmap-file header)
// 位图信息头 (bitmap-information header)
// 调色板     (color table), 24位以上没有调色板
// 位图数据   (bitmap-data)

// 以下结构体在 pshpack2.h 已经定义
typedef struct		// 位图文件头
{
	WORD   bfType;			// BMP 为 0x4D42 (BM的ascll码)
	DWORD  bfSize;			// 文件大小
	WORD   bfReserved1;		// 保留字, 置为0
	WORD   bfReserved2;		// 保留字, 置为0
	DWORD  bfOffBits;		// BMP 文件头大小
}BMP_FILE_HEADER;

typedef struct		// 位图信息头
{
	DWORD  biSize;			// 本结构的大小
	LONG   biWidth;			// 宽度
	LONG   biHeight;		// 高度
	WORD   biPlanes;		// 目标设备的级别
	WORD   biBitCount;		// 每个像素所需的位数
	DWORD  biCompression;	// 压缩类型，0为不压缩
	DWORD  biSizeImage;		// 位图的大小
	LONG   biXPelsPerMeter;	// 水平分辨率
	LONG   biYPelsPerMeter;	// 垂直分辨率
	DWORD  biClrUsed;		// 颜色表中的颜色数
	DWORD  biClrImportant;	// 重要的颜色数
}BMP_INFO_HEADER;
*/

typedef struct		// 位图数据
{
	BYTE b;
	BYTE g;
	BYTE r;
}pRGB;

// 填写好头信息, 数据信息, 写入文件
int write_pic(BYTE *pData, int width, int height, char *filename)
{	
	int size = width * height * 3;

	// 第一部分，文件头信息
	BITMAPFILEHEADER  bfh;
	bfh.bfType = 0x4d42;
	bfh.bfSize = size + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	bfh.bfReserved1 = 0;
	bfh.bfReserved2 = 0;
	bfh.bfOffBits = bfh.bfSize - size;
	
	// 第二部分，数据信息
	BITMAPINFOHEADER  bih;
	bih.biSize = sizeof(BITMAPINFOHEADER);
	bih.biWidth = width;
	bih.biHeight = height;
	bih.biPlanes = 1;
	bih.biBitCount = 24;	// 24 色的没有调色板
	bih.biCompression = 0;
	bih.biSizeImage = size;
	bih.biXPelsPerMeter = 0;
	bih.biYPelsPerMeter = 0;
	bih.biClrUsed = 0;
	bih.biClrImportant = 0;      
	
	FILE *fp = fopen(filename, "wb");
	if (!fp) return 0;

	// 写入文件
	fwrite(&bfh, sizeof(BITMAPFILEHEADER), 1, fp);
	fwrite(&bih, sizeof(BITMAPINFOHEADER), 1, fp);
	fwrite(pData, size, 1, fp);
	fclose(fp);

	return 1;
}

// 画横线, 绘制区域 i = is to ie, j = js to je
inline void draw_line(pRGB (*rgb)[48], int is, int js, int ie, int je)
{
	int i, j;

	if (is > ie) {i = is; is = ie; ie = i;}
	if (js > je) {i = js; js = je; je = i;}
	for (i=is; i<ie; i++)
	{
		for (j=js; j<je; j++)
		{
			rgb[i][j].r = 3;	// 画黑线
			rgb[i][j].g = 3;
			rgb[i][j].b = 3;
		}
	}
}

// 绘制单个数字
void draw_num(pRGB (*rgb)[48], int num, int s)
{
	int e = s + 10;
	switch (num)
	{
	case 0 : 
		draw_line(rgb, 3, s, 4, e-2);		// 下
		draw_line(rgb, 18, s, 19, e-2);		// 上
		draw_line(rgb, 3, s, 19, s+1);		// 左
		draw_line(rgb, 3, e-3, 19, e-2);	// 右
		break;
	case 1 : 
		draw_line(rgb, 3, e-3, 19, e-2);	// 右
		break;
	case 2 : 
		draw_line(rgb, 3, s, 4, e-2);		// 下
		draw_line(rgb, 11, s, 12, e-2);		// 中
		draw_line(rgb, 18, s, 19, e-2);		// 上
		draw_line(rgb, 3, s, 11, s+1);		// 左下
		draw_line(rgb, 12, e-3, 19, e-2);	// 右上
		break;
	case 3 : 
		draw_line(rgb, 3, s, 4, e-2);		// 下
		draw_line(rgb, 11, s, 12, e-2);		// 中
		draw_line(rgb, 18, s, 19, e-2);		// 上
		draw_line(rgb, 3, e-3, 19, e-2);	// 右
		break;
	case 4 : 
		draw_line(rgb, 11, s, 12, e-2);		// 中
		draw_line(rgb, 3, e-3, 19, e-2);	// 右
		draw_line(rgb, 12, s, 19, s+1);		// 左上
		break;
	case 5 : 
		draw_line(rgb, 3, s, 4, e-2);		// 下
		draw_line(rgb, 11, s, 12, e-2);		// 中
		draw_line(rgb, 18, s, 19, e-2);		// 上
		draw_line(rgb, 12, s, 19, s+1);		// 左上
		draw_line(rgb, 3, e-3, 11, e-2);	// 右下
		break;
	case 6 : 
		draw_line(rgb, 3, s, 4, e-2);		// 下
		draw_line(rgb, 11, s, 12, e-2);		// 中
		draw_line(rgb, 18, s, 19, e-2);		// 上
		draw_line(rgb, 3, s, 19, s+1);		// 左
		draw_line(rgb, 3, e-3, 11, e-2);	// 右下
		break;
	case 7 : 
		draw_line(rgb, 18, s, 19, e-2);		// 上
		draw_line(rgb, 3, e-3, 19, e-2);	// 右
		break;
	case 8 : 
		draw_line(rgb, 3, s, 4, e-2);		// 下
		draw_line(rgb, 11, s, 12, e-2);		// 中
		draw_line(rgb, 18, s, 19, e-2);		// 上
		draw_line(rgb, 3, s, 19, s+1);		// 左
		draw_line(rgb, 3, e-3, 19, e-2);	// 右
		break;
	case 9 : 
		draw_line(rgb, 3, s, 4, e-2);		// 下
		draw_line(rgb, 11, s, 12, e-2);		// 中
		draw_line(rgb, 18, s, 19, e-2);		// 上
		draw_line(rgb, 3, e-3, 19, e-2);	// 右
		draw_line(rgb, 12, s, 19, s+1);		// 左上
		break;
	default : break;
	}
}

// 在 path 生成验证码, 错误返回 -1
// 生成算法: 
// 随机画上四个数字
// 随机生成干扰点
int create_pic(char *path)
{
	pRGB rgb[22][48];	// 定义位图数据
	int id = 0;			// 验证码
	int i, r;
	int x, y;
	int pos = 4;
	
	srand(time(NULL));
	memset(rgb, 250, sizeof(rgb));		// 250 底色略白

	// 数字绘图区域 i = 3 to 19, j = 4 to 44
	// 每个数字 22 * 10
	for (i=0; i<4; i++)		// 随机四个数字
	{
		r = rand() % 10;
		id = id * 10 + r;
		draw_num(rgb, r, pos);
		pos += 10;
	}
	for (i=0; i<150; i++)	// 随机生成 n 个干扰点
	{
		x = rand() % 22;
		y = rand() % 48;
		rgb[x][y].r = rand() % 256;
		rgb[x][y].g = rand() % 256;
		rgb[x][y].b = rand() % 256;
	}
	if (!write_pic((BYTE *)rgb, 48, 22, path)) return -1;

	return id;
}

int main(void)
{
	int k = create_pic("c:\\rgb.bmp");
	printf("验证码: %d\n", k);

	return 0;
}
