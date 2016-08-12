
// 多重背包问题, 完全背包中每件物品可以拿无限件
// 多重背包中, 第 i 件物品可以拿 count[i] 件, 有限件

#include <stdio.h>

#define NUM 50
#define MAX(a, b) ((a) > (b) ? (a) : (b))

int dp(int c, int n, int *w, int *v, int *cnt)
{
	int sum[NUM][NUM];
	int i, j, k, h;

	for (k=0, i=0; i<=n; i++)
	{
		for (j=0; j<=c; j++) sum[i][j] = 0;
		k += cnt[i];		// 共有 k 件物品
	}
	for (i=1; i<=k; i++)
	{
		for (j=0; j<n; j++)			// 拿第 j 件物品
		{ if (cnt[j]) { cnt[j]--;  break; } }
		for (h=0; h<=c; h++)
		{
			if (w[j] <= h)			// 转化为 0-1 背包
				sum[i][h] = MAX(sum[i-1][h], sum[i-1][h-w[j]] + v[j]);
			else sum[i][h] = sum[i-1][h];
		}
	}
	return sum[k][c];
}

int main(void)
{
	int c, n;		// 最大容量 c, 总数 n
	int weight[NUM] = {0};
	int value[NUM] = {0};
	int count[NUM] = {0};
	int i, sum;

	printf("输入背包最大容量及物品总数: ");
	scanf("%d %d", &c, &n);
	printf("输入 %d 件物品: (重量 价值)\n", n);
	for (i=1; i<=n; i++)		// 0 号单元不用
		scanf("%d %d %d", weight + i, value + i, count + i);
	sum = dp(c, n, weight, value, count);
	printf("\n最大价值: %d\n", sum);
	return 0;
}
