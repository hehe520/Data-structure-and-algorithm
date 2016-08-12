
// 完全背包问题
// 0-1 背包中, 每件物品只允许拿一次, 而完全背包中, 每件物品允许拿无限次
// 主要函数
// dp()				普通DP,   时 O(n^2), 空 O(n^2+n)
// dp2()			空间优化  时 O(n^2), 空 O(n+n)
// dp3()			时空优化  时 O(n^2), 空 O(n)

#include <stdio.h>

#define SIZE 150
#define MAX(a, b) ((a) > (b) ? (a) : (b))

// 解法一: 转化为 0-1 背包来做
// 每件物品拿的件数不能超过背包的重量 c
// 即每件物品最多只能拿 c / w[i] 次
// 于是每件物品被拆成了许多件, 再按照 0-1 背包来做

// 状态方程: sum[i][j] = MAX{sum[i-1][j], sum[i][ j- w[k] ] + v[k]}
// 第 i 件物品, 不拿 i 时, 解 = sum[i-1][j]
// 拿 i 时, 解 = sum[i][ j- w[k] ] + v[k]
int dp(int c, int n, int *w, int *v)
{
	// sum[a][b] = c 第 a 件物品, 在容量 b 时, 最大价值为 c
	int sum[SIZE][SIZE];
	int count[SIZE];		// 统计每件物品可拿的次数
	int i, j, k, total = 0;

	for (i=0; i<=c; i++) sum[0][i] = 0;		// 初始化 sum
	for (i=1; i<=n; i++)			// 先求出每件物品可拿的次数
	{
		count[i] = c / w[i];
		total += count[i];			// 总件数
	}
	for (i=1; i<=total; i++)		// 第 i 次拿物品
	{
		for (k=1; k<=n; k++)		// 拿第 k 件物品
		{ if (count[k]) { count[k]--;   break; }}
		for (j=0; j<=c; j++)		// 枚举容量
		{	
			// 拿第 k 件
			if (w[k] > j) sum[i][j] = sum[i-1][j];	// 放不下
			else sum[i][j] = MAX(sum[i-1][j], sum[i][ j-w[k] ] + v[k]);
		}
	}
	return sum[total][c];		// 第 total 件物品
}


// dp2() 在 dp() 的基础上, 用滚动数组优化空间
int dp2(int c, int n, int *w, int *v)
{
	// sum[a] = c, 拿 a 件物品, 最大价值为 c
	int sum[SIZE];			// sum[] 优化为一维数组
	int count[SIZE];
	int i, j, k, total = 0;

	for (i=0; i<=c; i++) sum[i] = 0;
	for (i=1; i<=n; i++)
	{
		count[i] = c / w[i];
		total += count[i];			// 总件数
	}
	for (i=0; i<total; i++)			// 第 i 次拿物品
	{
		for (k=1; k<=n; k++)		// 拿第 k 件物品
		{ if (count[k]) {count[k]--;   break;}}
		// 枚举容量, 必须从后向前推
		// 因为拿第 k 个物品的时候需要, sum[k] 和 sum[ j-w[k] ] 的值
		// 从前往后推, 会覆盖掉 sum[j-w[k]]
		for (j=c; j>=0; j--)
		{
			if (w[k] <= j)		// 装得下
				sum[j] = MAX(sum[j], sum[ j-w[k] ] + v[k]);
		}	// else 装不下就还是原来的 sum[k]
	}
	return sum[c];		// 第 total 件物品
}


// 大牛解法
// 0-1背包中, 枚举容量之所以容量要从 C -> 0 递推
// 是因为要保证 sum[c] 是从 sum[c-w[k]] 递推而来
// 即要保证 sum[c-w[k]] 是前 k 件物品的最优解, 没有被物品 k 破坏
// 而完全背包中每件物品可以无限拿, 那么 sum[c-w[k]] 可以包含当前物品 k
// 所以容量必须从 0 -> C 枚举, sum[c-w[k]] 可能加上了当前物品
int dp3(int c, int n, int *w, int *v)
{
	int sum[SIZE];
	int i, j;

	for (i=0; i<=c; i++) sum[i] = 0;
	for (i=0; i<n; i++)			// 遍历 n 件物品 0 -> n
		for (j=0; j<=c; j++)		// 枚举容量 0 -> C
			if (w[i] <= j)  sum[j] = MAX(sum[j], sum[j-w[i]] + v[i]);
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
	sum = dp2(c, n, weight, value);
//	sum = dp3(c, n, weight, value);
	printf("\n最大价值: %d\n", sum);
	return 0;
}
