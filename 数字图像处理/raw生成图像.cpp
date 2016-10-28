
// raw 格式直接保存图像数据, 打开的时候要指定宽高

#include <stdio.h>

typedef unsigned char UC;

int main(void)
{
	FILE *fp = fopen("41035_H1.raw", "wb");
	UC data[256];
	int i;

	if (fp != NULL)
	{
		// data 只保存 0 - 256, 再写 64 次就是 256 * 64 了
		for (i=0; i<256; i++) data[i] = (UC)i;
		for (i=0; i<64; i++)
		{
			// fwrite() 一次写入的数据量有限, 所以这里分行写入
			// 好像超过 64KB 就会写入失败, 返回 0
			fwrite(data, 1, 256, fp);
		}
		fclose(fp);
	}
	else printf("打开文件错误\n");
	return 0;
}