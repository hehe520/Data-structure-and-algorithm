
// 二分图的最大匹配 - 最大流法 - 详见 <算法导论> 409 页
// 在二分图上加上 s, t 点, 求一次最大流 (预留推进或增广路)
// 在残余网络中有反向流量的边, 就是最大匹配数

// 数据的输入
// 如果按照图的的输入方式, 不能保证输入的是二分图
// 又要另外要加一个染色法判断二分图
// 所以输入边集(a, b)时, 约定 a 属于 s 集, b 属于 t 集

#include <stdio.h>
#include <queue>

#define NUM  80			// 最大顶点数
#define NUM2 300		// 最大边数
#define INF 0xFFFFF
#define MIN(a, b) ((a) > (b) ? (b) : (a))

struct		// 边集数组保存二分图
{
	int a;
	int b;
}e[NUM2];
int top = 0;

typedef struct node		// 队列结点
{
	int i;
	int e;
	int h;
	bool operator < (const node &a) const
	{ return (h < a.h); }
}node;

// 广搜计算高度
void height(int (*can)[NUM], int n, node *w, int t)
{
	std::queue<int> q;
	int used[NUM];
	int i, tmp;

	for (i=0; i<n; i++) used[i] = 0;
	w[t].h = 0;
	q.push(t);
	used[t] = 1;
	while (!q.empty())
	{
		tmp = q.front();   q.pop();
		for (i=0; i<n; i++)
		{
			if (!used[i] && can[i][tmp] > 0)
			{
				w[i].h = w[tmp].h + 1;
				q.push(i);
				used[i] = 1;
			}
		}
	}
}

// 预留推进求最大流 + 优先队列优化
void max_flow(int (*can)[NUM], int n, int s, int t)
{
	std::priority_queue<node *> q;
	node w[NUM];		// 记录结点余量, 高度
	node *tmp;
	int min;
	int f = 1;
	int i;

	height(can, n, w, t);		// 初始化 w[]
	for (i=0; i<n; i++)
	{
		w[i].i = i;
		w[i].e = 0;
	}
	w[s].h++;
	w[s].e = INF;
	q.push(w + s);
	while (!q.empty())		// 进行推进
	{
		tmp = q.top();   q.pop();
		if (f) f = 0;
		else if (tmp->i == s || tmp->i == t) continue;
		for (i=0; i<n; i++)
		{		// 有路走, 且高度比 i 高, 进行推进
			if (can[tmp->i][i] != 0 && tmp->h > w[i].h)
			{
				min = MIN(can[tmp->i][i], tmp->e);
				can[tmp->i][i] -= min;
				can[i][tmp->i] += min;
				tmp->e -= min;
				w[i].e += min;
				if (i != t || i != s) q.push(w + i);
			}
		}
		// tmp 结点的余量未推进完
		if (tmp->i != s && tmp->e > 0)
		{
			min = INF;
			for (i=0; i<n; i++)
				if (can[tmp->i][i] != 0 && w[i].h < min)
					min = w[i].h;
			tmp->h = min + 1;
			q.push(tmp);
		}
	}
}

// 最大二分匹配, 用优先队列的预留推进
// 算法流程:
// 根据边集数组构造残余网络, 当前网络, 包含源点汇点
// 预留推进求最大流
// 最后去当前网络中找有流量经过的边, 统计数量
int pipei(int n)
{
	int count = 0;			// 匹配数量
	int can[NUM][NUM];		// 残余网络
	int i, j;
	int s = n++;		// 增加源点 n-2
	int t = n++;		// 增加汇点 n-1
	int a, b;

	for (i=0; i<n; i++)		// 初始化
		for (j=0; j<n; j++)
			can[i][j] = 0;
	for (i=0; i<top; i++)	// 构造残余网络
	{
		a = e[i].a;   b = e[i].b;
		can[a][b] = 1;
		can[s][a] = 1;
		can[b][t] = 1;
	}
	max_flow(can, n, s, t);		// 进行一次最大流
	for (i=0; i<top; i++)
	{
		a = e[i].a;   b = e[i].b;
		if (can[b][a] > 0)		// 有反向流量
		{
			printf("(%d, %d), ", a, b);
			count++;
		}
	}
	return count;
}

int main(void)
{
	int n;
	int a, b;
	int max;

	printf("输入顶点数及边集: ");
	scanf("%d", &n);
	while (1)
	{
		scanf("%d %d", &a, &b);
		if (a == 0 && b == 0) break;
		e[top].a = a;
		e[top++].b = b;
	}
	printf("\n匹配的边: ");
	max = pipei(n);
	printf("\n最大二分匹配数: %d\n", max);

	return 0;
}