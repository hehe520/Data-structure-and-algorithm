#include <stdio.h>
#include <string.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define SIZE 200

// res 记录 str1, str2 匹配结果
int dp(char *str1, int len1, char *str2, int len2, int (*res)[SIZE])
{
	int i, j;
	int m = MAX(len1, len2);

	// 0 号单元放 0
	for (i=0; i<=m; i++) { res[0][i] = 0;  res[i][0] = 0; }
	for (i=0; i<len1; i++)
	{
		for (j=0; j<len2; j++)
		{
			if (str1[i] == str2[j]) 
				res[i+1][j+1] = res[i][j] + 1;
			else 
				res[i+1][j+1] = MAX(res[i+1][j], res[i][j+1]);
			// 记录前面匹配成功的最大长度, 因为不要求连续
		}
	}
/*	// 查看 dp 的记录矩阵
	printf("%4c", ' ');
	for (i=0; i<len2; i++) printf("%4c", str2[i]);
	printf("\n");
	for (i=1; i<=len1; i++)
	{
		printf("%4c", str1[i-1]);
		for (j=1; j<=len2; j++)
			printf("%4d", res[i][j]);
		printf("\n");
	}*/
	return res[len1][len2];
}

// 输出一个最长公共子序列
void show_str(char *str1, int x, char *str2, int y, int (*res)[SIZE])
{
	char stack[SIZE];
	int top = SIZE;		// 从高地址向低地址生长, 类似8086方式

	stack[--top] = '\0';
	while (x && y)
	{
		if (res[x-1][y] == res[x][y-1])
		{
			if (res[x-1][y-1] == res[x-1][y])	// 该点相等
			{
				if (str1[x-1] == str2[y-1])
					stack[--top] = str1[x-1];
				x--;   y--;
			}
			else y--;
		}
		else if (res[x-1][y] > res[x][y-1]) x--;
		else y--;
	}
	printf("%s\n", stack + top);
}

int main(void)
{
	char str1[SIZE];
	char str2[SIZE];
	int res[SIZE][SIZE];
	int x, y;
	int max_lenth;

	printf("输入两个字符串: ");
	scanf("%s", str1);
	scanf("%s", str2);
	x = strlen(str1);
	y = strlen(str2);
	max_lenth = dp(str1, x, str2, y, res);
	if (max_lenth)
	{
		printf("\n最大长度: %d", max_lenth);
		printf("\n子序列: ");
		show_str(str1, x, str2, y, res);
	}
	else printf("\n没有公共子序列");
	printf("\n");
	
	return 0;
}
