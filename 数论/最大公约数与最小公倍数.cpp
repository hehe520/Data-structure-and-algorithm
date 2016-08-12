
// 最大公约数 - 欧几里德算法
// gcd(a, b) = gcd(b, a mod b)
// lcm(a, b) = (a * b) / gcd(a, b)

#include <stdio.h>

long int gcd(long int a, long int b)
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

long int lcm(long int a, long int b)
{
	return ((a * b) / gcd(a, b));
}

int main(void)
{
	long int a, b;

	printf("输入两个数: ");
	scanf("%ld %ld", &a, &b);
	printf("\n最大公约数: %ld\n", gcd(a, b));
	printf("最小公倍数: %ld\n", lcm(a, b));

	return 0;
}