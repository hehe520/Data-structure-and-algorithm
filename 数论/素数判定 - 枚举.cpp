
// 对于 n, 试除 2 到 sqrt(n)

#include <stdio.h>
#include <math.h>

int judge(long int a)
{
	int m;
	int i;

	m = (int)sqrt(a);
	for (i=2; i<=m; i++)
		if (a % i == 0) return 0;
	return 1;
}

int main(void)
{
	long int i;

	printf("素数:\n");
	for (i=2; i<200; i++)
	{
		if (judge(i)) printf("%ld, ", i);
	}
	return 0;
}