#include <stdio.h>
#include <math.h>

// 递推法
int eular(int n)
{
	int m = (int)sqrt(n);
	int ans = 1;
	int i;

	for (i=2; i<=m; i++)
	{
		if (n % i == 0)
		{
			n /= i;
			ans *= i - 1;
			while (n % i == 0)
			{ n /= i;  ans *= i; }
		}
	}
	if (n > 1) ans *= n-1;

	return ans;
}

int main(void)
{
	int i;

	printf("100 以内欧拉函数表:\n");
	for (i=2; i<=100; i++)
	{
		printf("%-6d%-6d\n", i, eular(i));
	}
	return 0;
}