#include <stdio.h>

// 递归法求 fibonacci 数
__int64 fib(__int64 n)
{
	if (n == 1 || n == 2) return 1;
	else return fib(n-1) + fib(n-2);
}

int main(void)
{
	int n;

	printf("求fibonacci数, 请输入一个 n: ");
	scanf("%d", &n);
	if (n <= 0) printf("\n输入错误!\n");
	else printf("\n结果: %ld\n", fib(n));

	return 0;
}