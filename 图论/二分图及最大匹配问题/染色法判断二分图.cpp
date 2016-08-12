
// 染色法判断二分图
// 选一个点染成白色, 从该点发起一个广搜
// 将其邻接点染成相反的黑色
// 若发现邻接点是白色, 则表示不是二分图
// 直到广搜结束, 则是二分图

#include <stdio.h>
#include <queue>

#define NUM 50			// 最大顶点数
#define WRITE 0			// 白色
#define BLACK 1			// 黑色
#define NO -1			// 还没染色

int color[NUM];			// 结点颜色

// 随机选一个点, 进行广搜染色
// 此次广搜不需要 used[] 访问标记, 因为没有染过色的就是没访问的
int BFS(short int (*m)[NUM], int n)
{
	int i;
	int result;
	int BFS_s(short int (*m)[NUM], int n, int i);	// 打通单个连通分支

	for (i=0; i<n; i++) color[i] = NO;		// 没染色
	for (i=0; i<n; i++)
	{
		if (color[i] == NO) result = BFS_s(m, n, i);
		if (result == 0) return 0;		// 搜索失败
	}
	return 1;
}

// 判断单个连通分支
int BFS_s(short int (*m)[NUM], int n, int i)
{
	std::queue<int> q;
	int tmp, j;
	int c;

	color[i] = WRITE;		// 首次染白色
	q.push(i);
	while (!q.empty())
	{
		tmp = q.front();   q.pop();
		c = color[tmp] == WRITE ? BLACK : WRITE;
		// c 取相反的颜色
		for (j=0; j<n; j++)
		{
			if (m[tmp][j] == 1)		// 如果有路
			{
				if (color[j] == NO)		// 未染过
				{
					color[j] = c;
					q.push(j);
				}
				else if (color[j] == color[tmp]) 
					return 0;			// 颜色冲突
			}
		}
	}
	return 1;
}

int main(void)
{
	short int m[NUM][NUM];		// 邻接矩阵
	int n;
	int i, j;
	int a, b;

	printf("输入顶点数: ");
	scanf("%d", &n);
	for (i=0; i<n; i++)
		for (j=0; j<n; j++) m[i][j] = 0;
	while (1)
	{
		scanf("%d %d", &a, &b);
		if (a == 0 && b == 0) break;
		m[a][b] = m[b][a] = 1;
	}
	if (BFS(m, n))
	{
		printf("\n这是二分图");
		printf("\n A 集: ");
		for (i=0; i<n; i++)
			if (color[i] == WRITE) 
				printf("%d, ", i);
		printf("\n B 集: ");
		for (i=0; i<n; i++)
			if (color[i] == BLACK) 
				printf("%d, ", i);
	}
	else printf("\n不是二分图");
	putchar('\n');

	return 0;
}
