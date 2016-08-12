
// 线性不定方程 ax + by = c
// 输入 a, b, c, 输出 x, y

#include <stdio.h>

int gcd(int a, int b)		// 最大公约数
{
	long int c;

	while (b)
	{
		c = a % b;
		a = b;
		b = c;
	}
	return a;
}

// 拓展的欧几里德算法, 详细证明见算法导论 528 页
void extand_gcd(int a, int b, int &d, int &x, int &y)
{
	if (b == 0) {d = a;  x = 1;  y = 0; }
	else 
	{
		extand_gcd(b, a % b, d, y, x);
		y -= x * (a / b);
	}
}

// 计算 ax + by = c
int calc(int a, int b, int c, int &x, int &y)
{
	int d = gcd(a, b);

	// c 要是 gcd(a, b) 的倍数, 否则无解
	// 即, 从 a, b 能提出公因子与 c 抵消
	if (c % d != 0) return 0;

	// 将 ax + by = c 转化为 ax + by = d 计算
	extand_gcd(a, b, d, x, y);
	c /= d;
	x *= c;   y *= c;
	return 1;
}

int main(void)
{
	int a, b, c, x, y;

	while (1)
	{
		printf("输入 a, b, c: ");
		scanf("%d %d %d", &a, &b, &c);
		if (calc(a, b, c, x, y))
			printf("x = %d   y = %d\n\n", x, y);
		else 
			printf("方程无解!\n\n");
	}
	return 0;
}