#include <stdio.h>

// 文件复制, 将 c:\a.txt 复制到 c:\b.txt
#define SPATH "c:\\a.txt"
#define DPATH "c:\\b.txt"
#define NUM 4069		// 4 KB

int main(void)
{
	FILE *in = NULL;
	FILE *out = NULL;
	char buffer[NUM];		// 4 KB 的缓冲区
	int count;

	in = fopen(SPATH, "rt");
	if (in == NULL) { printf("打开文件失败!\n");  return 0; }

	out = fopen(DPATH, "wt");
	if (out == NULL) { printf("写入文件失败!\n"); return 0; }

	while (1)
	{		// 每次读取一个字节, 读取 NUM 次, count 记录读取成功的次数
		count = fread(buffer, 1, NUM, in);
		if (count <= 0) break;			// 复制完了, 或者读取失败了
		count = fwrite(buffer, 1, count, out);
		if (count <= 0) { printf("写入失败!\n");   break; }
	}
	if (in != NULL) { fclose(in);   in = NULL; }
	if (out != NULL) { fclose(out);   out = NULL; }
	printf("复制成功\n");
	return 0;
}
