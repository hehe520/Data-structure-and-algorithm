
// RMQ 问题, 找出某区间的最值
// 有 n 个数, m 次查询, 普通遍历要 O(n*m)
// ST 算法能达到 O(m log n), 在 O(1) 时间内找出最值

// 引进一个 max(i, j) 数组记录最值
// max(i, j) 表示 i 到 i+2^j 区间内的最值
// DP状态转移方程: max(i, j) = MAXIMUM{max(i, j-1), max(i+2^(j-1), j-1)}
// 例如有 3 2 4 5 6 8 1 2 9 7 序列
// max(0, 2) = MAXIMUM(max(0, 1), max(1, 1))
// 3 2 4 5 6 =         3 2 4 5  , 2 4 5 6

// RMQ(i, j) 表示查询 i 到 j 的最值
// k = log2(区间长度);
// RMQ(i, j) = MAXIMUM{max(i, k), max(j-2^k+1, k)}
// 例如 RMQ(0, 4) = MAXIMUM(max(0, 2), max(1, 2))

#include <stdio.h>
#include <math.h>

#define NUM 400			// 最多 400 个数字
#define MAXIMUM(a, b) ((a) > (b) ? (a) : (b))
// 如果要求区间最小值, 只需更改此宏

int num[NUM];		// 数组
int n;
int max[NUM][NUM];	// 记录最优值

// DP 计算最优值
void initialize()
{
	int i, j;
	int k = (int)(log(n) / log(2));

	for (i=1; i<=n; i++) max[i][0] = num[i];
	// 区间 i 到 i 的最值显然是 num[i]

	for (j=1; j<=n-(j<<1)+1; j++)	// j < n-2^j+1
	{
		for (i=1; i+(1<<j)-1<=n; i++)
		{
			max[i][j] = MAXIMUM(max[i][j-1], max[i+(1<<(j-1))][j-1]);
		}
	}
}

int RMQ(int a, int b)
{
	int k = (int)(log(b-a+1) / log(2));
	// log(长度) / log(2) = log2(长度)
	// 函数库没有 log2() 函数, 所以用了一个对数的换底公式

	return MAXIMUM(max[a][k], max[b-(1<<k)+1][k]);
}

int main(void)
{
	int a, b;
	int i;

	printf("输入 n, 及 n 个数: \n");
	scanf("%d", &n);
	for (i=1; i<=n; i++)		// 下标从 1 开始
		scanf("%d", num + i);
	initialize();
	// 输入查询区间
	while (1)
	{
		scanf("%d %d", &a, &b);
		printf("最大值: %d\n", RMQ(a, b));
	}
	return 0;
}
/*		一组测试数据
10
3 2 4 5 6 8 1 2 9 7
*/