
#include <stdio.h>

typedef unsigned char WORD;		// 一个字节

// 文件结构
// P5		表示灰度图像
// x, y		高和宽
// max		最大颜色级度, [0..max] 之间取值
// 图像数据...

int main(void)
{
	FILE *fp = fopen("pic.ppm", "wb");
	WORD data[64][256];
	int i, j;

	if (fp != NULL)
	{
		// 生成图片
		for (i=0; i<64; i++)
			for (j=0; j<256; j++)
				data[i][j] = (WORD)j;

		// ppm 文件头, P5灰度图, P6彩色图
		fprintf(fp, "%s\n%u %u\n%u\n", "P5", 256, 64, 255);

		// ppm 数据部分
		fwrite(data, 1, 64 * 256 * sizeof(WORD), fp);
		fclose(fp);
	}

	// 读取图片
	char str[20];
	fp = fopen("pic.ppm", "rb");
	if (fp != NULL)
	{
		fgets(str, 20, fp);
		printf("图像类型: %s", str);
		fgets(str, 20, fp);
		printf("高与长: %s", str);
		fgets(str, 20, fp);
		printf("最大级度: %s\n", str);
	}
	return 0;
}