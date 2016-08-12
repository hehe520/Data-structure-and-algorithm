
// 普通的 prim 算法中是采用类似选择排序的方法
// 每次选择最小的 s 集到 t 集最小的边, 来进行松弛 O(n)
// 这里使用优先队列来维护这个选择的操作 O(log n)

#include <stdio.h>
#include <queue>

#define NUM 60
#define INF 0xFFFFFF

int m[NUM][NUM];	// 邻接矩阵
int n;

// 优先队列的结点
// s 集到 t 的最短边 v 最短优先
typedef struct node
{
	int i;		// 编号
	int v;		// 最短边
	bool operator < (const node &a) const
	{ return (v > a.v); }
}node;

// 使用优先队列优化 prim 算法
int prim()
{
	std::priority_queue<node> q;
	int dis[NUM];		// s 集到 t 集的最小距离
	int pre[NUM];		// 记录前驱
	int used[NUM];		// 使用标记
	int value = 0;
	int i, j;
	node tmp, tmp2;

	for (i=0; i<n; i++)		// 初始化
	{
		pre[i] = -1;
		dis[i] = INF;
		used[i] = 0;
	}
	tmp.i = 0;		// 设置起点为 0
	q.push(tmp);
	// n 个点需要松弛 n-1 次
	for (i=0; i<n; i++)
	{
		if (q.empty()) return -1;		// 不连通
		// 更新最小边的时候会产生很多重复的入队
		// 用 used[] 判断取出的结点是否使用过
		tmp = q.top();
		while (used[tmp.i])
		{
			q.pop();
			tmp = q.top();
		}
		q.pop();
		used[tmp.i] = 1;	// 找到最小的边 tmp.v
		if (tmp.i != 0)		// 不是起点 0
		{
			value += m[tmp.i][pre[tmp.i]];
			printf("(%d, %d), ", tmp.i, pre[tmp.i]);
		}
		for (j=0; j<n; j++)
		{
			if (!used[j] && m[tmp.i][j] < dis[j])
			{
				dis[j] = m[tmp.i][j];
				pre[j] = tmp.i;
				// (j, dis[j]) 被松弛后入队
				tmp2.i = j;
				tmp2.v = dis[j];
				q.push(tmp2);
			}
		}
	}
	return value;
}

int main(void)
{
	int i, j, v;
	
	printf("输入顶点个数: ");
	scanf("%d", &n);
	for (i=0; i<n; i++)
		for (j=0; j<n; j++) m[i][j] = INF;
	printf("输入边集: ");
	while (1)
	{
		scanf("%d %d %d", &i, &j, &v);
		if (!(i || j || v)) break;
		m[i][j] = v;
		m[j][i] = v;
	}
	printf("\n生成树边集: ");
	v = prim();
	printf("\n最小权值: %d\n", v);

	return 0;
}
/*		两组测试数据
6
0 1 6
0 5 1
0 4 5
1 5 5
1 2 3
2 5 6
2 3 6
3 5 4
3 4 2
4 5 5
0 0 0

9
0 1 5
1 2 7
2 3 26
3 4 8
4 5 11
5 6 20
6 0 10
0 7 1
1 7 22
2 7 4
8 7 2
5 7 21
6 7 8
2 8 15
3 8 9
4 8 3
5 8 6
0 0 0
*/