
// 求出无向图最小环

#include <stdio.h>

#define NUM 60
#define INF 0xFFFFFF

int m[NUM][NUM];		// 邻接矩阵
int n;
int dis[NUM][NUM];		// 最短距离
int pre[NUM][NUM];		// 记录路径
int stack[NUM];			// 记录环
int top;

// 求出最小环权值和路径
// 类似 flody 不断选取中间点 k 进行测试
// min{ dis[i][j] + m[i][k] + m[k][j] }
int min_h(void)
{
	int min = INF;
	int i, j, k;
	int tmp;

	for (i=0; i<n; i++)		// 初始化 dis, pre
	{
		for (j=0; j<n; j++)
		{
			dis[i][j] = m[i][j];
			if (m[i][j] != INF)
				pre[i][j] = i;
			else pre[i][j] = -1;
		}
	}
	for (k=0; k<n; k++)
	{
		for (i=0; i<n; i++)
		{
			for (j=0; j<n; j++)
			{
				// 找最小环, 同时记录该环
				if (dis[i][j] + m[i][k] + m[k][j] < min)
				{
					min = dis[i][j] + m[i][k] + m[k][j];
					// 记录 i 到 j 的路径, 增加 k
					top = 0;
					tmp = j;
					while (tmp != -1)
					{
						stack[top++] = tmp;
						tmp = pre[i][tmp];
					}
					stack[top++] = k;
				}
				// flody 部分, 求出 i 到 j 的最短路
				if (i != j && dis[i][k] + dis[k][j] < dis[i][j])
				{
					dis[i][j] = dis[i][k] + dis[k][j];
					pre[i][j] = pre[k][j];
					// 在 i 行 j 的前驱更新为 k 行 j 的前驱
				}
			}
		}
	}
	return min;
}

int main(void)
{
	int i, j, v;

	printf("输入顶点数及边集: ");
	scanf("%d", &n);
	if (n <= 2) printf("\n两个点构不成环!\n");
	for (i=0; i<n; i++)
		for (j=0; j<n; j++) m[i][j] = INF;
	while (1)
	{
		scanf("%d %d %d", &i, &j, &v);
		if (!(i || j || v)) break;
		m[i][j] = v;
		m[j][i] = v;
	}
	printf("\n最小环权值: %d\n点集: ", min_h());
	for (i=0; i<top; i++)
		printf("%d, ", stack[i]);
	printf("\n");

	return 0;
}
/*
6
0 1 9
0 2 1
2 3 1
3 4 1
4 1 1
0 3 6
3 1 5
5 0 1
5 1 1
0 0 0
*/