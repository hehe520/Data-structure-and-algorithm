
#include <stdio.h>
#include <string.h>

typedef unsigned char WORD;

typedef struct		// ppm 彩色像素点, 三个字节
{
	WORD r;
	WORD g;
	WORD b;
}ppm_RGB;

// 文件结构
// P6		表示彩色图像
// x, y		长和宽
// max		最大颜色级度, [0..max] 之间取值
// 图像数据...

int main(void)
{
	FILE *fp = fopen("pic.ppm", "wb");
	ppm_RGB data[160][128];
	int i, j;

	if (fp != NULL)
	{
		memset(data, 0, sizeof(data));

		for (i=0; i<20; i++)
			for (j=0; j<128; j++)
				data[i][j].r = 255;	// 红色
		for (i=20; i<40; i++)
			for (j=0; j<128; j++)
				data[i][j].g = 255;	// 绿色
		for (i=40; i<60; i++)
			for (j=0; j<128; j++)
				data[i][j].b = 255;	// 蓝色
		for (i=60; i<80; i++)
			for (j=0; j<128; j++)
				data[i][j].r = 255, data[i][j].g = 255;	// 红绿
		for (i=80; i<100; i++)
			for (j=0; j<128; j++)
				data[i][j].r = 255, data[i][j].b = 255;	// 红蓝
		for (i=100; i<120; i++)
			for (j=0; j<128; j++)
				data[i][j].b = 255, data[i][j].g = 255;	// 蓝绿
		for (i=120; i<140; i++)
		{
			for (j=0; j<128; j++)	// 黑色
			{
				data[i][j].r = 255;
				data[i][j].g = 255;
				data[i][j].b = 255;
			}
		}
		fprintf(fp, "%s\n%d %d\n%d\n", "P6", 160, 128, 255);
		// fwrite() 一次能写入的数据有限, 好像是 64K, 多了写入失败
		// 这里分四次写入
		for (i=0; i<4; i++)
			fwrite(data[i * 40], 1, 40 * 128 * sizeof(ppm_RGB), fp);
		fclose(fp);
	}
	return 0;
}