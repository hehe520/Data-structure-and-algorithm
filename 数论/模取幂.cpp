
// 模取幂, 求 a^n mod p 的值

// 算法1: 利用公式: (a * b) mod m = (a mod m) * (b mod m) mod m
// a^n = a^(n-1) * a
// 所以: a^n mod p = ((a^(n-1) mod p) * a mod p) mod p
//       a^(n-1) mod p = ((a^(n-2) mod p) * a mod p) mod p
// 进行 n 次迭代, 时间复杂度 O(n)

// 算法2: 将 n 用二进制表示, 按权展开
// 例如 a^6 mod p,  6 = (110)B = 1*2^2 + 1*2^1 + 0*2^0
// 所以 a^(1*2^2 + 1*2^1 + 0*2^0) = (a^1*2^2) * (a^1*2^1) * (a^0*2^0)
// 再利用公式: (a * b) mod p = ((a mod p) * b) mod p 进行迭代
// 6 是 110 有三个位, 迭代 3 次
// n 有 log n 个位, 时间复杂度 O(log n)

#include <stdio.h>
#include <time.h>

// 算法1
int calc(int a, int n, int p)
{
	int r = 1;		// r = a^0 = 1
	int i;

	for (i=0; i<n; i++)
	{
		r = (r % p * a % p) % p;
	}
	return r;
}

// 算法2
int calc2(int a, int n, int p)
{
	int r = 1;
	long int d = a % p;
	// d 为 a 的 2^i 次方模 p 的结果
	// d2 = (d1 * d1) % p;

	while (n)
	{
		if (n & 1) r = (r * d) % p;
		d = (d * d) % p;
		n >>= 1;
	}
	return r;
}

int main(void)
{
	int a, n, p;
	int t, t1, t2;

	while (1)
	{
		printf("输入底数 a: ");
		scanf("%d", &a);
		printf("输入指数 n: ");
		scanf("%d", &n);
		printf("输入 p: ");
		scanf("%d", &p);
		t1 = clock();
		t = calc(a, n, p);
		t2 = clock();
		printf("\n结果: %d\nO(n) 级用时: %d ms\n", t, t2 - t1);
		t1 = clock();
		t = calc2(a, n, p);
		t2 = clock();
		printf("\n结果: %d\nO(log n) 级用时: %d ms\n\n", t, t2 - t1);
	}
	return 0;
}