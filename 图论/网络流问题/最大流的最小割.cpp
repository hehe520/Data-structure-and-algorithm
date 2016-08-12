
// 找最小割, 最小割的容量等于最大流

#include <stdio.h>
#include <string.h>
#include <queue>

#define NUM 50			// 最大结点数
#define INF 0XFFFFFF

typedef struct node		// 优先队列的结点
{
	int i;		// 结点编号
	int e;		// 余量
	int h;		// 高度
	bool operator < (const node &a) const
	{ return (h < a.h); }
}node;

// 计算高度
void height(int (*r)[NUM], node *h, int n, int t)
{
	int used[NUM];
	int i, tmp;
	std::queue<int> q;

	for (i=0; i<n; i++) used[i] = 0;
	h[t].h = 0;			// 汇点高度最低
	q.push(t);
	used[t] = 1;
	while (!q.empty())
	{
		tmp = q.front();   q.pop();
		for (i=0; i<n; i++)
		{
			if (!used[i] && r[i][tmp] != 0)
			{
				h[i].h = h[tmp].h + 1;
				q.push(i);
				used[i] = 1;
}}}}

// 从 a 推向 b
inline void PUSH(int (*r)[NUM], node *w, int a, int b)
{
	int max;		// 最大推进量

	max = r[a][b] > w[a].e ? w[a].e : r[a][b];
	r[a][b] -= max;
	r[b][a] += max;
	w[a].e -= max;
	w[b].e += max;
}

// 优先队列 + 预流推进 求最大流, 同时输出 s 集, t 集
// 算法流程:
// 初始化每个结点的高度, 广搜
// 初始化源点信息, 并入队
// 从源点开始推进, 推进不完的重标记入队
// 直到队列为空
// 对 s 点进行广搜, 找出 s 集, 剩下的就是 t 集了
int maxflow(int (*m)[NUM], int n, int s, int t)
{
	std::priority_queue<node *> q;	// 优先队列
	std::queue<int> q2;
	int used[NUM];
	int r[NUM][NUM];
	node w[NUM];		// 记录结点信息, 高度, 余量等
	node *tmp;
	short int f = 1;
	int i, j;
	int min_h;

	for (i=0; i<n; i++)		// 不破坏 m[][], 初始化
	{
		for (j=0; j<n; j++) r[i][j] = m[i][j];
		w[i].i = i;
		w[i].e = 0;
	}
	height(r, w, n, t);
	w[s].e = INF;
	w[s].h++;
	q.push(w + s);
	while (!q.empty())
	{
		tmp = q.top();   q.pop();
		if (f) f = 0;
		else if (tmp->i == s || tmp->i == t) continue;
		for (i=0; i<n; i++)
		{
			if (r[tmp->i][i] != 0 && tmp->h > w[i].h)
			{
				PUSH(r, w, tmp->i, i);
				if (i != t && i != s) q.push(w + i);
			}
		}
		if (tmp->i != s && tmp->e > 0)		// 还有余量, 重标记入队 
		{
			min_h = INF;
			for (i=0; i<n; i++)
				if (r[tmp->i][i] != 0 && w[i].h < min_h) 
					min_h = w[i].h;
			tmp->h = min_h + 1;
			q.push(tmp);
		}
	}
	for (i=0; i<n; i++) used[i] = 0;
	q2.push(s);
	used[s] = 1;			// 从 s 点发起一次广搜
	while (!q2.empty())
	{
		j = q2.front();   q2.pop();
		for (i=0; i<n; i++)
		{
			if (!used[i] && r[j][i] != 0)
			{
				used[i] = 1;
				q2.push(i);
			}
		}
	}
	printf("\n s 集: ");
	for (i=0; i<n; i++)
		if (used[i]) printf("%d, ", i);
	printf("\n t 集: ");
	for (i=0; i<n; i++)
		if (!used[i]) printf("%d, ", i);
	return w[t].e;
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
	printf("\n最小割容量: %d\n", maxflow(matrix, n, a, b));

	return 0;
}
