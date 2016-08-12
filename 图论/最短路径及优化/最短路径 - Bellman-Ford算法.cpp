
// Bellman-ford 算法, 允许存在负边的图

#include <stdio.h>

#define NUM  50		// 最大结点数
#define NUM2 200	// 最大边数
#define INF  0xFFFFFf

struct		// 边集数组, (s, t) = v;
{
	int s;
	int t;
	int v;
}node[NUM2];
int top = 0;

// 把边集中的边, 进行 n-1 次松弛, 那么每个点松弛了 n-1 次
// n 个点中, 其中一个点到其余的点, 至多有 n-1 条边
// 如果还能松弛那就说明存在负权的边了
int bellman(int n, int start, int end, int *pre, int *r)
{							// pre 返回前驱信息, r 返回结果
	int dis[NUM];
	int i, j;
	int s, t, v;

	for (i=0; i<top; i++) dis[i] = INF;
	dis[start] = 0;
	
	for (i=1; i<n; i++)		// 每个点松弛 n-1 次
	{
		for (j=0; j<top; j++)	// 遍历边集
		{
			s = node[j].s;
			t = node[j].t;
			v = node[j].v;
			if (dis[s] + v < dis[t])
			{
				dis[t] = dis[s] + v;
				pre[t] = s;			// 记录前驱
			}
			// 如果 dis[s] + cost[s..t] < dis[t] 则更新 dis[t]
		}
	}
	for (j=0; j<top; j++)
		if (dis[node[j].s] + node[j].v < dis[node[j].t])
			return 0;
	*r = dis[end];
	return 1;
}

int main(void)
{
	int n;
	int a, b, c;		// (a, b) = c
	int pre[NUM];

	printf("输入顶点个数: ");
	scanf("%d", &n);
	while (1)
	{
		scanf("%d %d %d", &a, &b, &c);
		if (!(a || b || c)) break;
		pre[top] = -1;
		node[top].s = a;
		node[top].t = b;
		node[top++].v = c;
	}
	printf("输入起点, 终点: ");
	scanf("%d %d", &a, &b);
	if (bellman(n, a, b, pre, &c))
	{
		printf("\n最短路径长: %d\n", c);
		printf("路径: ");
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