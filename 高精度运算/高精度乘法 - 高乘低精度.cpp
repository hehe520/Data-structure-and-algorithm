
// 高精度乘以低精度
// 高精度范围 0 - 10^1000000
// 低精度范围 int 型之内

#include <stdio.h>
#include <string.h>

#define NUM 1000000

char a[NUM];		// 高精度乘数
char z[NUM];		// 保存结果
int top;

// 入口参数 a, b, 出口参数 r + top
void mul(int b)
{
	int i;
	int len = strlen(a);
	int cf = 0;
	int tmp = 0;

	top = NUM;
	z[--top] = '\0';
	for (i=len-1; i>=0; i--)
	{
		tmp = (a[i] - '0') * b + cf;
		z[--top] = tmp % 10 + '0';
		cf = tmp / 10;
	}
	while (cf)
	{
		z[--top] = cf % 10 + '0';
		cf /= 10;
	}
	while (z[top] == '0') top++;	// 消除前导 0
	if (z[top] == '\0') z[--top] = '0';
}

int main(void)
{
	int b;

	while (1)
	{
		printf("输入高精度整数: ");
		scanf("%s", a);
		printf("输入低精度整数: ");
		scanf("%d", &b);
		mul(b);		// a * b
		printf("结果: %s\n\n", z + top);
	}
	return 0;
}