
// 最小费用的最大流
// 每条边不仅有流量, 还有费用(权)
// 找 s 到 t 的最大流, 同时还要费用最小

// 普通的最大流用广搜, 深搜, dijskral 等等查找增广路
// 而这里要求费用最小, 每次要找一条费用最小的增广路
// 用 dijskral 或者 SPFA 来找费用最小的增广路
// 进行 n 次贪心后, 直到找不到增广路了
// 此时当前网络中, 有流量通过的边加起来, 就是最小费用了

#include <stdio.h>
#include <queue>

#define NUM 50		// 最大顶点数
#define INF 0xFFFFFF

typedef struct
{
	int v;		// 流量
	int c;		// 费用 cost
}matrix;	// 邻接矩阵

// SPFA 查找费用最小的路径, 假设不存在负环
int SPFA(matrix (*e)[NUM], int n, int *pre, int s, int t)
{
	std::queue<int> q;
	int used[NUM];		// 在不在队列中
	int dis[NUM];		// 最短距离
	int i, tmp;

	for (i=0; i<n; i++) {
		pre[i] = -1;
		used[i] = 0;
		dis[i] = INF;
	}
	dis[s] = 0;
	q.push(s);
	used[s] = 1;
	while (!q.empty())
	{
		tmp = q.front();   q.pop();
		for (i=0; i<n; i++)			// 松弛邻接点
		{
			// SPFA 稍作修改, 除了 dis[tmp] + e[tmp][i] < dis[i]
			// 还要残余网络 e[tmp][i] 中的流量不为 0, 即有路可走
			// 因为如果 tmp -> i 流量是 0 了
			// 再松弛 tmp -> i 该段, 已经没用了
			// 这样只会造成增广路上有一段路径是 0, 增广量为 0
			if (e[tmp][i].v != 0 
				&& dis[tmp] + e[tmp][i].c < dis[i])
			{
				dis[i] = dis[tmp] + e[tmp][i].c;
				pre[i] = tmp;
				if (!used[i])		// 被松弛的点入队一次
				{
					q.push(i);
					used[i] = 1;
				}
			}
		}
	}
	if (pre[t] != -1) return 1;
	else return 0;
}

// 求最小费用, max_f 返回最大流
// 算法流程:
// 用 SPFA 查找一条费用最小的增广路
// 沿增广路更新残余网络, 当前网络
// 直到找不到增广路为止
// 在当前网络中统计最小费用, 最大流
int min_cost(matrix (*m)[NUM], int n, int s, int t, int *max_f)
{
	matrix e[NUM][NUM];		// 残余网络
	matrix c[NUM][NUM];		// 当前网络
	int pre[NUM];
	int cur;
	int mincost = 0;
	int min;
	int i, j;

	for (i=0; i<n; i++) {		// 初始化
		for (j=0; j<n; j++) {
			e[i][j] = m[i][j];		// 不破坏 m 数据
			c[i][j].c = m[i][j].c;
			c[i][j].v = 0;
		}
	}
	while (SPFA(e, n, pre, s, t))	// 如果能找到增广路
	{
		min = INF;
		cur = t;
		while (pre[cur] != -1)		// 找最小的流量
		{
			if (e[pre[cur]][cur].v < min)
				min = e[pre[cur]][cur].v;
			cur = pre[cur];
		}
		cur = t;
		while (pre[cur] != -1)		// 进行增广
		{
			i = pre[cur];
			e[i][cur].v -= min;		// 更新残余网络
			e[cur][i].v += min;		// 记录反向流量
			c[i][cur].v += min;		// 更新当前网络
			c[cur][i].v -= min;
			cur = pre[cur];
		}
	}
	// 在当前网络中统计最小费用, 最大流
	*max_f = 0;
	for (i=0; i<n; i++) 
	{
		if (c[i][t].v != 0) (*max_f) += c[i][t].v;
		for (j=0; j<n; j++)
		{
			if (c[i][j].v > 0) mincost += c[i][j].c;
		}
	}
	return mincost;
}

int main(void)
{
	int a, b, v, c;		// (a, b) 流量 v, 费用 c
	int n;
	int i, j;
	int max_f;
	matrix m[NUM][NUM];		// 邻接矩阵

	printf("输入顶点个数: ");
	scanf("%d", &n);
	for (i=0; i<n; i++) {
		for (j=0; j<n; j++) {
			m[i][j].v = 0;		// 初始流量为 0
			m[i][j].c = INF;	// 费用无限大
		}
	}
	printf("输入边集: ");
	while (1)
	{
		scanf("%d %d %d %d", &a, &b, &v, &c);
		if (!(a || b || v || c)) break;
		m[a][b].v = v;
		m[a][b].c = c;
	}
	printf("输入源点, 汇点: ");
	scanf("%d %d", &a, &b);
	printf("\n最小费用: %d\n", min_cost(m, n, a, b, &max_f));
	printf("最大流: %d\n", max_f);

	return 0;
}
