
// 高精度数除以低精度数, 不支持负数小数
// 算法: 模拟人工计算过程
// 主要函数:
// div()		产生商和余数
// div2()		产生有小数的商

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM 1000000
// 高精度范围 0 - 10^1000000
// 低精度范围 0 - 99999999

// 计算 a / b
char a[NUM];		// 被除数
int  b;				// 除数
char c[NUM];		// 商
char r[NUM];		// 余数, 或小数部分

int intlen(int n)		// 求 int 的位数
{
	int len = 0;

	while (n) 
	{
		len++;
		n /= 10;
	}
	return len;
}

// 清除前导 0
void clear_zero(char *c, int topc)
{
	int i, j;

	for (i=0; i<topc; i++)		// 消除前导 0
	{
		if (c[i] != '0') break;
	}
	if (i != 0)
	{
		for (j=0; i<topc; j++, i++)
			c[j] = c[i];
		c[j] = '\0';
	}
	else c[topc] = '\0';
	if (c[0] == '\0') { c[1] = c[0];   c[0] = '0'; }
}

// 入口参数, a, b, 出口参数: c, r
void div()
{
	int lena, lenb;
	int tmp, i;
	int cur;
	int topc = 0;

	// 比较 a, b 的大小
	lena = strlen(a);
	lenb = intlen(b);
	// 如果 lena <= lenb 说明在 int 的表示范围内
	if (lena <= lenb)
	{
		tmp = atoi(a);
		itoa(tmp / b, c, 10);
		itoa(tmp % b, r, 10);
		return ;
	}
	// 此时 lena > lenb, a 比 b 大
	for (tmp=i=0, cur=NULL; i<lenb; i++)
		tmp = tmp * 10 + a[cur++] - '0';
	while (1)
	{
		c[topc++] = tmp / b + '0';
		tmp %= b;
		if (a[cur] == '\0') break;
		tmp = tmp * 10 + a[cur++] - '0';
	}
	itoa(tmp, r, 10);		// 余数
	clear_zero(c, topc);	// 清除前导 0
}

// 入口参数, a, b, n, 出口参数: c, r
// n 表示计算到第 n 位小数
// 在的余数中, 再不断地乘 10 相除
void div2(__int64 n)
{
	int lena, lenb;
	int topa, topc, topr;
	int i, tmp;

	c[0] = r[0] = '\0';
	lena = strlen(a);
	lenb = intlen(b);
	for (tmp=topa=i=0; i<lenb; i++)
	{
		if (topa < lena)
			tmp = tmp * 10 + a[topa++] - '0';
		else break;
	}
	topc = 0;
	while (1)
	{
		c[topc++] = tmp / b + '0';
		tmp %= b;
		if (a[topa] == '\0') break;
		tmp = tmp * 10 + a[topa++] - '0';
	}
	clear_zero(c, topc);		// 清除前导 0
	for (topr=i=0; i<n; i++)	// 计算 n 位小数
	{
		tmp = tmp * 10;
		r[topr++] = tmp / b + '0';
		tmp %= b;
	}
	r[topr] = '\0';
	// 清除后导 0, 只保留一个 0
	while (r[topr - 1] == '0' && topr != 1) topr--;
	r[topr] = '\0';
}

int main(void)
{
	while (1)
	{
		printf("输入高精度被除数: ");
		scanf("%s", a);
		printf("输入低精度除数: ");
		scanf("%d", &b);
		if (b == 0) return 0;		// 不能除 0
		div();
		printf("\n商: %s\n余数: %s\n", c, r);
		div2(40);			// 计算 40 位小数
		printf("小数形式: %s.%s\n\n", c, r);
	}
	return 0;
}