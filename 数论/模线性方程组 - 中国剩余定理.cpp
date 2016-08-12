
// 方程组 x ≡ a[] (mod m[]), 其中 m[] 两两互素
// 输入 n, a[], m[] 输出一个 x, 

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

// 拓展的欧几里德算法
void extand_gcd(int a, int b, int &d, int &x, int &y)
{
	if (b == 0) { d = a;  x = 1;  y = 0; }
	else 
	{
		extand_gcd(b, a % b, d, y, x);
		y -= x * (a / b);
	}
}

// 解模线性方程组, 
int calc(int *a, int *m, int n, int &M)
{
	int i, w, d, x = 0, y;

	M = 1;
	for (i=0; i<n; i++) M *= m[i];
	for (i=0; i<n; i++)
	{
		w = M / m[i];
		extand_gcd(m[i], w, d, d, y);
		x = (x + y * w * a[i]) % M;
	}
	return (x + M) % M;
}

int main(void)
{
	int a[100];
	int m[100];
	int n, i, x, M;

	while (1)
	{
		printf("输入方程组的个数 n: ");
		scanf("%d", &n);
		printf("输入a[] :");
		for (i=0; i<n; i++)
			scanf("%d", a + i);
		printf("输入m[] :");
		for (i=0; i<n; i++)
			scanf("%d", m + i);
		x = calc(a, m, n, M);
		printf("结果: %d + %dk\n\n", x, M);
	}
	return 0;
}