
// warshall算法, 求一个图的传递闭包
// 传递闭包: 加入最少的边, 使得该图具有传递性

#include <stdio.h>

#define NUM 60

void solve(int (*m)[NUM], int n)
{
	int i, j, k;

	// i, j 循环是遍历矩阵的
	// k 层循环, 拿第 k 个点进行试探, k 点作为中间点, 如 a -> k -> b
	// 那么 a -> b 有传递
	// k 层循环类似 dijskral 的松弛操作, 类似 flody 的取 k 点进行全网松弛
	for (k=0; k<n; k++)
		for (i=0; i<n; i++)
			for (j=0; j<n; j++)
				m[i][j] = m[i][j] | (m[i][k] & m[k][j]);
				// 如果 i->j 本身有路, 或者 i->k->j 有路
				// 那么 i->j 有路
}

int main(void)
{
	int m[NUM][NUM];		// 邻接矩阵
	int n;
	int a, b;
	int i, j;

	printf("输入顶点个数: ");
	scanf("%d", &n);
	for (i=0; i<n; i++)
		for (j=0; j<n; j++) m[i][j] = 0;
	while (1)
	{
		scanf("%d %d", &a, &b);
		if (a == 0 && b == 0) break;
		m[a][b] = 1;		// 有向图
	}
	// 求传递闭包, 求完之后 m[][] 被修改
	solve(m, n);
	printf("\n传递闭包:\n");
	for (i=0; i<n; i++)
	{
		for (j=0; j<n; j++) 
			printf("%3d ", m[i][j]);
		putchar('\n');
	}
	return 0;
}
/*
4
0 1
1 2
2 3
3 2
0 2
0 0
*/