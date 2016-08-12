
// 最大流问题, 增广路算法, Ford-Fulkerson 算法

#include <stdio.h>
#include <string.h>
#include <queue>

#define NUM 30		// 最大顶点数

// 算法流程:
// 在残余网络中, 通过广搜找到一条增广路
// 找出增广路上的最小权值的边
// 更新残余网络
// 直到找不到增广路为止
// 统计当前网络中汇点的流
int maxflow(int (*m)[NUM], int n, const int s, const int e)
{
	int r[NUM][NUM];		// 残余网络
	int max_flow = 0;
	int i, j;
	int cur, min;
	std::queue<int> q;		// 广搜队列
	char used[NUM];
	int pre[NUM];

	for (i=0; i<n; i++)			// 初始化
		for (j=0; j<n; j++)
			r[i][j] = m[i][j];		// r 是为了不破坏main中 m 的数据
	while (1)		// 不断查找增广路
	{
		// 广搜一条增广路, 找不到就 break
		memset(used, 0, sizeof(used));
		memset(pre, -1, sizeof(pre));
		q.push(s);
		used[s] = 1;
		cur = s;
		while (!q.empty())		// 在 r 中查找一个增广路
		{
			cur = q.front();   q.pop();
			if (cur == e) break;
			for (i=0; i<n; i++)
			{
				if (!used[i] && r[cur][i] != 0)
				{
					used[i] = 1;
					q.push(i);
					pre[i] = cur;
					if (i == e) break;		// 找到了
				}
			}
		}
		if (pre[e] == -1) break;		// 没有增广路了
		// 根据前驱信息 pre 从后往前找最小权值
		cur = e;
		min = 0XFFFFFF;
		while (pre[cur] != -1)
		{
			if (r[ pre[cur] ][cur] < min)
				min = r[ pre[cur] ][cur];
			cur = pre[cur];
		}
		// 更新网络, 当前网络中加上 min, 如果当前
		// 反向加上 min, 指明流的来向, 有可能被其它流抵消
		cur = e;
		while (pre[cur] != -1)
		{
			r[ pre[cur] ][cur] -= min;		// 残余网络
			r[cur][ pre[cur] ] += min;
			cur = pre[cur];
		}
	}
	// 根据残余网络的信息统计流量, 如果 i->e 有路, 统计 e 来自i的流量
	for (i=0; i<n; i++)
		if (m[i][e] != 0) max_flow += r[e][i];
	return max_flow;
}

int main(void)
{
	int n;
	int matrix[NUM][NUM];		// 邻接矩阵
	int a, b, c;

	memset(matrix, 0, sizeof(matrix));
	printf("请输入一个带权有向图: ");
	printf("顶点个数: ");
	scanf("%d", &n);
	printf("边集: ");
	while (1)
	{
		scanf("%d %d %d", &a, &b, &c);
		if (!(a || b || c)) break;
		matrix[a][b] = c;
	}
	printf("输入源点和汇点: ");
	scanf("%d %d", &a, &b);
	printf("\n最大流: %d\n", maxflow(matrix, n, a, b));
	return 0;
}
