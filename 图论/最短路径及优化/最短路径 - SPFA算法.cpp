
// SPFA 算法
// 在 bellman-Ford 算法上改进, 用队列优化, 避免了不必要的松弛
// SPFA 算法和广搜类似, 可以松弛的结点在队列中, 其余结点不在队列中
// 做一个改进, 使得 SPFA 算法能够处理负环, 记录每个结点的如果次数
// 如果超过了 n-1 次, 那么就存在负环了
// 因为 n 个点中, 其中一个点到其余的点至多有 n-1 条边
// 也就是最多只能松弛 n-1 次, 最多入队 n-1 次

#include <stdio.h>
#include <queue>

#define NUM  50		// 最大结点数
#define INF  0xFFFFFf

// 初始化, 起点入队
// 从队列中取出一个结点, 松弛邻接点, 被松驰的点入队
int SPFA(int(*m)[NUM], int n, int start, int end, int *pre, int *r)
{										// pre 返回前驱, r 返回最短路径
	int used[NUM];		// 标志在不在队列中
	int dis[NUM];		// 最短距离
	int count[NUM];		// 统计结点入队次数, 判断负环
	std::queue<int> q;
	int i, tmp;
	
	for (i=0; i<n; i++)
	{
		used[i] = 0;
		dis[i] = INF;
		count[i] = 0;
	}
	dis[start] = 0;
	q.push(start);
	count[start]++;
	used[start] = 1;
	while (!q.empty())
	{
		tmp = q.front();   q.pop();
		for (i=0; i<n; i++)		// 遍历 tmp 邻接点
		{
			if (dis[tmp] + m[tmp][i] < dis[i])
			{
				dis[i] = dis[tmp] + m[tmp][i];
				pre[i] = tmp;
				if (!used[i])		// 如果不在队列中, 则入队
				{
					count[i]++;		// 判断负环
					if (count[i] >= n) return 0;
					used[i] = 1;
					q.push(i);
				}
			}
		}
		used[tmp] = 0;		// 表示不在队列中了
	}
	*r = dis[end];
	return 1;
}

int main(void)
{
	int n;
	int a, b, c;		// (a, b) = c
	int m[NUM][NUM];
	int pre[NUM];		// 记录前驱

	printf("输入顶点个数: ");
	scanf("%d", &n);
	for (a=0; a<n; a++) {
		for (b=0; b<n; b++) m[a][b] = INF;
		pre[a] = -1;
	}
	while (1)
	{
		scanf("%d %d %d", &a, &b, &c);
		if (!(a || b || c)) break;
		m[a][b] = c;
	}
	printf("输入起点, 终点: ");
	scanf("%d %d", &a, &b);
	if (SPFA(m, n, a, b, pre, &c))
	{
		printf("\n最短路径长: %d\n路径: ", c);
		c = b;
		while (c != -1)
		{
			printf("%d <- ", c);
			c = pre[c];
		}
		printf("起点\n");
	}
	else printf("\n存在负环!\n");

	return 0;
}
/*   两组测试数据, 一组有解, 一组有负环
5
0 1 6
0 2 7
1 2 8
1 3 5
3 1 -2
1 4 -4
2 3 -3
2 4 9
4 3 7
0 0 0
0 4

4
0 1 1
1 2 -2
2 3 -3
3 2 -4
2 4 1
0 0 0
0 2
*/