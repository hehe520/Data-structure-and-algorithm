
// 最大流问题之 dinic 算法
// EK 算法是找广搜增广路, 进行增广
// 而 dinic 先广搜高度标号, 再用深搜找增广路
// 查找的条件是 height[a] == height[b] + 1
// 这就保证了流只会从高往低推进

#include <stdio.h>
#include <queue>

#define NUM 60
#define INF 0xFFFFFF

int m[NUM][NUM];		// 邻接矩阵
int n;
int h[NUM];			// 高度函数
int used[NUM];

void BFS(int s, int t)		// 广搜计算结点高度
{
	int i, tmp;
	std::queue<int> q;

	for (i=0; i<n; i++) used[i] = 0;
	h[t] = 0;		// 汇点最低
	used[t] = 1;
	q.push(t);
	while (!q.empty())
	{
		tmp = q.front();
		q.pop();
		for (i=0; i<n; i++)
		{
			// 从汇点往回搜
			if (!used[i] && m[i][tmp] != 0)
			{
				h[i] = h[tmp] + 1;
				used[i] = 1;
				q.push(i);
			}
		}
	}
}

// 从 cur 开始搜, 搜到 t 停止, 推进量 v
// 返回本次的推进量 min{v, cur->t}
// 找不到增广路返回 0
int DFS(int cur, int t, int v)
{
	int i;
	int f;		// 最大推进量

	if (cur == t) return v;
	for (i=0; i<n; i++)
	{							// 此处 h[cur] > h[i] 是重点
		if (!used[i] && m[cur][i] != 0 && h[cur] == h[i] + 1)
		{
			used[i] = 1;
			f = v > m[cur][i] ? m[cur][i] : v;
			f = DFS(i, t, f);		// 推进
			if (f == 0) break;		// 没增广路了
			m[cur][i] -= f;			// 回溯时, 记录
			m[i][cur] += f;			// 反向记录

			return f;
		}
	}
	return 0;
}

int dinic(int s, int t)
{
	int maxflow = 0;
	int tmp, i;

	while (1)
	{
		BFS(s, t);			// 广搜高度标号, 每次都要重新计算高度
		for (i=0; i<n; i++) used[i] = 0;
		tmp = DFS(s, t, INF);	// 深搜尝试增广 INF 个流量
		if (tmp) maxflow += tmp;
		else break;
	}
	return maxflow;
}

int main(void)
{
	int i, j, v;

	printf("输入顶点个数及边集: ");
	scanf("%d", &n);
	for (i=0; i<n; i++)
		for (j=0; j<n; j++) m[i][j] = 0;
	while (1)
	{
		scanf("%d %d %d", &i, &j, &v);
		if (!(i || j || v)) break;
		m[i][j] = v;
	}
	printf("输入源点汇点: ");
	scanf("%d %d", &i, &j);
	printf("\n最大流: %d\n", dinic(i, j));

	return 0;
}