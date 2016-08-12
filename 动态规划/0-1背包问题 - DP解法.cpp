#include <stdio.h>

// 主要函数
// dp()				普通DP,   时 O(n^2), 空 O(n^2)
// dp2()			空间优化  时 O(n^2), 空 O(n)
// show_and_dp()	输出拿了哪些物品

#define SIZE 300	// 最多物品数
#define MAX(a, b) ((a) > (b) ? (a) : (b))

// 0-1背包, DP解法
// 算法流程, 从第一个物品开始选择放或不放, 直到最后
// 在 sum[][] 数组中记录最优值
// sum[a][b] = c  含义:
// a 第 a 个物品,  b 在最大容量为 b 的时候, c 最优价值
int dp(int c, int n, int *w, int *v)
{
	int sum[SIZE][SIZE];		// 记录最优值
	int i, j;

	for (i=0; i<=n; i++)		// 初始化
		for (j=0; j<=c; j++) sum[i][j] = 0;
	
	for (i=1; i<=n; i++)	// 物品
	{
		for (j=1; j<=c; j++)	// 当前容量 j
		{
			// 如果当前背包容量小于第 i 件物品容量, 即放不下
			// 直接用前 i - 1 的最优值
			// 否则, 对于当前物品可以选择放或不放
			if (j < w[i]) sum[i][j] = sum[i-1][j];
			else sum[i][j] = MAX(sum[i-1][j], sum[i-1][ j - w[i] ] + v[i]);
			// 直接用前一件物品(i-1)的最优值 sum[i-1][j], 就是不放
			// 如果要放当前物品 i, 用当前容量 j, 减去自己容量 w[i], 腾出空间
			// 再从容量为 j-w[i] 的最优值, 加上自己的 v[i], 选出最大的
		}
	}
	return sum[n][c];		// n 个物品容量为 c 时的解
}

// 同时输出选了什么物品 (重量, 价值)
int show_and_dp(int c, int n, int *w, int *v)
{
	int sum[SIZE][SIZE];
	int used[SIZE][SIZE];	// used[][] == 0 没拿, == 1 拿了
	int i, j;

	for (i=0; i<=n; i++)
		for (j=0; j<=c; j++)
			sum[i][j] = used[i][j] = 0;

	for (i=1; i<=n; i++)	// 第 i 件物品
	{
		for (j=1; j<=c; j++)	// 容量 j
		{
			if (j < w[i]) sum[i][j] = sum[i-1][j];
			else 
			{	// 比较拿和不拿哪个大
				if (sum[i-1][j] >= sum[i-1][j - w[i]] + v[i])
					sum[i][j] = sum[i-1][j];
				else 
				{
					sum[i][j] = sum[i-1][j - w[i]] + v[i];
					used[i][j] = 1;		// 拿了
				}
			}
		}
	}
	printf("\n选了: ");
	i = n;   j = c;
	while (i >= 0)
	{
		if (used[i][j])
		{
			printf("(%d, %d), ", w[i], v[i]);
			j -= w[i];
		}
		i--;
	}
	return sum[n][c];
}

// 方法2求解, 优化空间复杂度 O(n)
// 对于不再保存 i 个物品, 容量是 j 的时候最优值是多少
// 只保存当前物品 i 的最优值
// 背包容量从后往前推, for j=c to 1 do
int dp2(int c, int n, int *w, int *v)
{
	int sum[SIZE];
	int i, j;

	for (i=0; i<=c; i++) sum[i] = 0;

	for (i=1; i<=n; i++)	// 第 i 件物品
	{
		for (j=c; j>=1; j--)	// 容量 j
		{
			if (j >= w[i])
				sum[j] = MAX(sum[j], sum[j - w[i]] + v[i]);
			// 只有当前容量 j 大于自己 w[i] 才能放得下
			// 否则 sum[j] = sum[j] 的值, 无变化
		}
		for (j=1; j<=c; j++) printf("%d, ", sum[j]);
		printf("\n");
	}
	return sum[c];
}

int main(void)
{
	int c, n;		// 最大容量 c, 总数 n
	int weight[SIZE] = {0};
	int value[SIZE] = {0};
	int i, sum;

	printf("输入背包最大容量及物品总数: ");
	scanf("%d %d", &c, &n);
	printf("输入 %d 件物品: (重量 价值)\n", n);
	for (i=1; i<=n; i++)		// 0 号单元不用
		scanf("%d %d", weight + i, value + i);
//	sum = dp(c, n, weight, value);
//	sum = dp2(c, n, weight, value);
	sum = show_and_dp(c, n, weight, value);   // 同时输出被选物品
	printf("\n最大价值: %d\n", sum);
	return 0;
}
