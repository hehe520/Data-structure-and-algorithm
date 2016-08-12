#include <stdio.h>

// 求 x^n
// 普通做法: x 自乘 n-1 次, 复杂度 O(n)
// 利用公式, 如果 n 是偶数 x^n = x^(n/2) * x^(n/2) = (x * x)^(n / 2)
// 如果 n 是奇数 x^n = x^((n-1)/2) * x^((n-1)/2) * x = (x * x)^((n-1)/2) * x
// 复杂度降到 O(log n) 递归树的深度

// 求 x^n 时间复杂度 O(log n)
double pow(int x, int n)
{
	if (n == 0) return 1;
	else if (n == 1) return x;
	else if (n % 2 == 0) return pow(x * x, n/2);
	else return pow(x * x, n/2) * x;
}

int main(void)
{
	int x, n;

	printf("输入底数和指数: ");
	scanf("%d %d", &x, &n);
	printf("\n结果: %0.lf\n", pow(x, n));

	return 0;
}
