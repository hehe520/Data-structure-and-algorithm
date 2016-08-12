
// 广搜, 求一个图的传递闭包
// 传递闭包: 加入最少的边, 使得该图具有传递性

#include <stdio.h>
#include <queue>

#define NUM 60

// 广搜
void BFS(int (*m)[NUM], int cur, int n)
{
	std::queue<int> q;
	int used[NUM];
	int i, tmp;

	for (i=0; i<n; i++) used[i] = 0;
	q.push(cur);
	// used[cur] = 1;	// 该句话不能有, 因为有可能传递到自己 (自反性)
	while (!q.empty())
	{
		tmp = q.front();
		q.pop();
		for (i=0; i<n; i++)
		{
			if (!used[i] && m[tmp][i] == 1)
			{
				q.push(i);
				used[i] = 1;
				m[cur][i] = 1;		// 标记有路
			}
		}
	}
}

// 对每一个点, 发起一个广搜, 连通所有搜到的点
// 因为有传递性才能达到该点, 到达了该点, 说明传递
void solve(int (*m)[NUM], int n)
{
	int i;
	for (i=0; i<n; i++) BFS(m, i, n);
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