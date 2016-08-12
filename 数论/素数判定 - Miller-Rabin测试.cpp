
#include <stdio.h>

// 快速模取幂 a^n mod p
int mod(int a, int n, int p)
{
	int r = 1;
	int d = a % p;

	while (n)
	{
		if (n & 1) r = (r * d) % p;
		d = (d * d) % p;
		n >>= 1;
	}
	return r;
}

// 用 a 随机测试 n 是不是素数, n > 7
int Miller_Rabin(int n, int a)
{
	int r = 0;
	int s = n - 1;
	int j, k;

	if (n % a == 0) return 0;
	while (s & 1 == 0)
	{
		s >>= 1;
		r++;
	}
	k = mod(a, s, n);
	if (k == 1) return 1;
	for (j=0; j<r; j++, k = k*k%n)
		if (k == n - 1) return 1;
	return 0;
}

int judge(int s)
{
	if (s <= 7)
	{
		if (s == 2 
			|| s == 3 
			|| s == 5 
			|| s == 7) return 1;
		return 0;
	}
	else
	{
		if (!Miller_Rabin(s, 2)) return 0;
		if (!Miller_Rabin(s, 3)) return 0;
		if (!Miller_Rabin(s, 5)) return 0;
		if (!Miller_Rabin(s, 7)) return 0;
		return 1;
	}
}

int main(void)
{
	int i;

	printf("素数:\n");
	for (i=2; i<200; i++)
	{
		if (judge(i)) printf("%d, ", i);
	}
	putchar('\n');
	return 0;
}
