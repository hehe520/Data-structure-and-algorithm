#include <stdio.h>

// 递归法, 求 n 的阶乘
// 这显然是一个很糟糕的算法, 用递推公式更快
__int64 jie(__int64 n)
{
	if (n == 0) return 1;
	else return n * jie(n-1);
}

int main(void)
{
	int n;

	printf("求 n 的阶乘, 请输入n: ");
	scanf("%d", &n);
	if (n < 0 || n >= 17) printf("\n结果超出范围!\n");
	else printf("\n结果: %ld\n", jie(n));

	return 0;
}
