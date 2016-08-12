
#include <stdio.h>
#include <algorithm>

#define MAX(a, b) ((a) > (b) ? (a) : (b))

int dp(int c, int n, int *w, int *v)
{
	int i, j;
	int sum[100005];

	for (i=1; i<=c; i++) sum[i] = 0;
	sum[0] = 1;
	for (i=0; i<n; i++)			// 遍历 n 件物品 0 -> n
		for (j=w[i]; j<=c; j++)		// 枚举容量 0 -> C
			sum[j] = MAX(sum[j], sum[j-w[i]] + v[i]);
	std::sort(sum, sum + c + 1);
	return sum[c] - 1;
}

int main(void)
{
	int c, n;		// 最大容量 c, 总数 n
	int weight[110];
	int value[110];
	int i, sum;
	
	while (scanf("%d", &n) != EOF)
	{
		for (i=0; i<n; i++)
			scanf("%d %d", value + i, weight + i);
		scanf("%d", &c);
		sum = dp(c, n, weight, value);
		printf("%d\n", sum);
	}
	return 0;
}
/*
3
3 3
7 7
9 9
10
5
1 1
5 3
10 3
6 8
7 5
6
*/