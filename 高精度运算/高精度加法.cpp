
// 高精度整型加法运算, 不支持小数, 负数可以转化为减法

#include <stdio.h>
#include <string.h>

#define NUM 10000000		// 精度 10^10000000 之内
// 高精度不可以无限高, 受到内存的限制

// -------------- z = x + y, 字符数组存储 ------------------
char x[NUM];
char y[NUM];
char z[NUM];

void add(void)
{
	int len1 = strlen(x);
	int len2 = strlen(y);
	int min = len1 > len2 ? len2 : len1;
	int cf = 0;			// 进位标志
	int top = NUM - 1;
	int tmp;
	int a, b, i;

	// 频繁调用 printf() 花时间, 用 z 保存结果, 一次输出
	z[top] = '\0';
	for (i=min-1; i>=0; i--)
	{
		a = x[--len1] - '0';
		b = y[--len2] - '0';
		tmp = (a + b + cf) % 10;
		cf = (a + b + cf) / 10;
		z[--top] = tmp + '0';
	}
	if (len1) 
	{
		// 不能把超出的部分直接复制到 z
		// 比如  99999000
		// 加        1000
		// 等于 100000000
		while (cf != 0)
		{
			if (len1 > 0)
				a = x[--len1] - '0';
			else a = 0;
			tmp = (a + cf) % 10;
			z[--top] = tmp + '0';
			cf = (a + cf) / 10;
		}
		while (len1) 
			z[--top] = x[--len1];
	}
	else if (len2)
	{
		while (cf != 0)
		{
			if (len2 > 0)
				a = y[--len2] - '0';
			else a = 0;
			tmp = (a + cf) % 10;
			z[--top] = tmp + '0';
			cf = (a + cf) / 10;
		}
		while (len2) 
			z[--top] = y[--len2];
	}
	if (cf) z[--top] = '1';
	printf("等于 :  %s\n", z + top);
}

int main(void)
{
	while (1)
	{
		printf("\n输入两个数:\n第一个: ");
		scanf("%s", x);
		printf("第二个: ");
		scanf("%s", y);

		add();
	}
	return 0;
}