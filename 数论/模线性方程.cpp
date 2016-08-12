
// 求解模线性方程 ax ≡ b(mod n)
// 输入 a, b, n, 输出所有的 x 解

#include <stdio.h>

int stack[100];
int top;

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

// 拓展的欧几里德算法
void extand_gcd(int a, int b, int &d, int &x, int &y)
{
	if (b == 0) {d = a;  x = 1;  y = 0; }
	else 
	{
		extand_gcd(b, a % b, d, y, x);
		y -= x * (a / b);
	}
}

// 计算 ax ≡ b(mod n), 结果放在 stack 中
void calc(int a, int b, int n)
{
	int d = gcd(a, b);
	int i, x, y;

	top = 0;
	extand_gcd(a, n, d, x, y);
	if (b % d != 0) return ;

	x = x * (b / d) % n;
	for (i=1; i<d; i++)
		stack[top++] = x + i * (n / d);
}

int main(void)
{
	int a, b, n;

	while (1)
	{
		printf("输入 a, b, n: ");
		scanf("%d %d %d", &a, &b, &n);
		calc(a, b, n);
		if (top)
		{
			printf("结果: ");
			while (top) printf("%d, ", stack[--top]);
			printf("\n\n");
		}
		else printf("方程无解!\n\n");
	}
	return 0;
}