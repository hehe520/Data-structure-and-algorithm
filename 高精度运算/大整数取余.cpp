
// 公式1: (a * b) mod m = ((a mod m) * (b mod m)) mod m
// 公式2: (a + b) mod m = ((a mod m) + (b mod m)) mod m
// 输入大正数 p, 正数 m, 求 p mod m

#include <stdio.h>

// p[0] 只储存一个十进制位
short int p[1000000];		// 100 万位
int len;
int m;

int main(void)
{
	int i, c;
	char t;

	printf("输入 p: ");
	while (scanf("%c", &t) != EOF)
	{
		if (t == '\n') break;
		p[len++] = t - '0';
	}
	printf("输入 m: ");
	scanf("%d", &m);
	for (c=i=0; i<len; i++)
	{
		c = (c * 10 + p[i]) % m;	// 公式 2
	}
	printf("余数: %d\n", c);

	return 0;
}