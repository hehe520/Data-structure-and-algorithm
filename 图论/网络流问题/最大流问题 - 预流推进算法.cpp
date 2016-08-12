
// 最大流问题, 预流推进算法

// maxflow()		使用普通队列进行推进
// maxflow2()		使用优先队列进行推进, 结点高度优先 (快)

// 增广路算法找最大流, 是不断的从源点到汇点找增广路进行的
// 而预流推进算法则是模拟水流, 一个一个结点推进的, 该算法快
// 假设从源点流进无限多的水, 最后看汇点能有多少水流出
// 水往低处流, 为了能使水能从源点流向汇点, 需要一个高度函数 height[i] 来说明结点的高度
// 并规定当 height[a] > height[b] 时, 水才能从 a 流向 b, 
// 高度函数保证了水流在网络中不会出现循环流动造成的死循环
// 显然 height[源点] = 无限高, height[汇点] = 0, 而中间结点的高度是到汇点的广搜距离

// 假设源点有无限多的水量, 所以需要一个函数 e[i] 来表示某个结点的水量
// 从源点开始, 把源点的邻接点, 全部灌满水, 并入队
// 再把邻接点中水, 向邻接点的低处推进, height[] 比我小的地方
// 如果能全部推进完 (e[i] == 0) 那最好, 如果不能推进完
// 提高该结点的高度, 重新入队, 进行下一次推进
// a 点提高的高度 height[a] = min{ height[邻接点] } + 1
// 重标记后就保证了至少有一个地方可流, 也可能会流回源点
// 因为网络中的结点, 不断被提高高度, 使得汇点饱和后, 剩下的残余流就只能流回源点了

#include <stdio.h>
#include <string.h>
#include <queue>

using namespace std;

#define NUM 50		// 最大顶点数
#define INF 0xFFFFFF

// 广搜计算每个结点的高度, 从汇点开始
void BFS(int (*m)[NUM], int *h, int n, int t)
{
	std::queue<int> q;
	short int used[NUM];
	int cur, i;

	memset(used, 0, sizeof(used));
	h[t] = 0;		// 汇点高度最低
	q.push(t);
	used[t] = 1;
	while (!q.empty()) {
		cur = q.front();   q.pop();
		for (i=0; i<n; i++) {
			if (!used[i] && m[i][cur] != 0) {
				h[i] = h[cur] + 1;
				used[i] = 1;
				q.push(i);
}}}}

// 一次水量推进过程, 将 a 的水推向 b
inline void PUSH(int (*r)[NUM], int *e, int a, int b)
{
	int min;		// 推进量

	// 计算 a->b 的推进量, 尽可能的推进, 看看余量和管道哪个小
	min = e[a] > r[a][b] ? r[a][b] : e[a];
	e[b] += min;		// 修改余量
	e[a] -= min;
	r[a][b] -= min;
	r[b][a] += min;		// 记录反向流量, 可能被更大的流推回来
}

// 算法流程:
// 广搜计算每个结点的高度
// 把源点的邻接点全部灌满, 并入队
// 从队中取出一个结点, 向低洼的点推进
// 如果该点还有余量 (cap[i] == 0), 提高高度, 重新入队
// 直到队列为空, 去汇点统计最大流
int maxflow(int (*m)[NUM], int n, int s, int t)
{
	int h[NUM];			// 高度函数
	int e[NUM];			// 结点余量
	int r[NUM][NUM];	// m[][] 的副本
	queue<int> q;
	int i, j, tmp, min;
	short int f = 1;

	memset(e, 0, sizeof(e));
	for (i=0; i<n; i++)			// 复制多一份 m[][]
		for (j=0; j<n; j++) r[i][j] = m[i][j];
	BFS(m, h, n, t);		// 计算每个结点的高度
	e[s] = INF;				// 源点无限多的水
	h[s]++;					// 源点无限高
	// s 可能会有高度相等的结点, 再提高 1, 就是所有结点中最高的了
	q.push(s);
	while (!q.empty())		// 普通队列处理
	{
		// 取出一个结点, 进行推进
		tmp = q.front();   q.pop();
		if (f) f = 0;				// 第一次源点可以向邻接点推进
		else if (tmp == t || tmp == s) continue;	// 不是源点汇点
		for (i=0; i<n; i++)
		{					// tmp 比 i 高, 才可以推进
			if (r[tmp][i] != 0 && h[tmp] > h[i])
			{
				PUSH(r, e, tmp, i);		// 推进 tmp -> i
				if (i != t) q.push(i);
			}	// 汇点 t 没必要再入队进行推进了, 入队后高度也是最低的, 无法推进
		}
		// 如果 tmp 没有推进完, 要提高高度, 准备再次推进
		// 随着高度不断增加, 余量要么推到汇点, 要么推回源点
		if (tmp != s && e[tmp] != 0)	// 源点无限多的水, 不能再标记了
		{
			min = INF;
			for (i=0; i<n; i++)		// 找一个最低的邻接点
				if (r[tmp][i] != 0 && h[i] < min)
					min = h[i];
			h[tmp] = min + 1;		// 提高一个高度
			q.push(tmp);		// 重入队
		}
	}
	return e[t];	// 汇点处的余量就是最大流了
}


// --------------------------------------------------------------------------
// ---------------------- maxflow2() 优先队列推进 ---------------------------
// --------------------------------------------------------------------------
// maxflow2() 和上面的 maxflow() 一样, 只不过 maxflow2() 使用优先队列处理
// 按照结点高度优先, 这就导致了处在地势最高的点, 最先被向邻接点放水
// 水不会从低处流向高处, 所以结点被重标记的几率小了很多
// 使用优先队列比普通队列快

typedef struct node		// 优先队列的结点
{
	int i;		// 结点号
	int h;		// 结点高度
	int e;		// 结点余量
	bool operator < (const node &b) const	// 比较器, 优先队列使用
	{ return (h < b.h); }
}node;

// 因为用了结构体打包h[], e[], 不得不再重写一遍 BFS()
void BFS2(int (*m)[NUM], node *re, int n, int t)
{
	std::queue<int> q;
	short int used[NUM];
	int cur, i;

	memset(used, 0, sizeof(used));
	re[t].h = 0;		// 汇点高度最低
	q.push(t);
	used[t] = 1;
	while (!q.empty()) {
		cur = q.front();   q.pop();
		for (i=0; i<n; i++) {
			if (!used[i] && m[i][cur] != 0) {
				re[i].h = re[cur].h + 1;
				used[i] = 1;
				q.push(i);
}}}}
// 重写一遍 PUSH(), a 向 b 推进
inline void PUSH2(int (*r)[NUM], node *w, int a, int b)
{
	// 计算推进量 min, s 点的余量与管道容量比较
	int min = r[a][b] > w[a].e ? w[a].e : r[a][b];
	w[a].e -= min;
	w[b].e += min;
	r[a][b] -= min;
	r[b][a] += min;		// 记录反向流量
}

// 开始 maxflow2(), 结点n 个, 源点 s, 汇点 t
int maxflow2(int (*m)[NUM], int n, int s, int t)
{
	priority_queue<node *> q;		// 优先队列
	int r[NUM][NUM];			// m[][] 的副本
	node w[NUM];		// 记录结点高度, 余量, 也就是用结构体打了个包
	node *tmp;
	int i, j;
	short int f = 1;
	int min;

	for (i=0; i<n; i++)		// 复制多一份 m[][]
	{
		for (j=0; j<n; j++) r[i][j] = m[i][j];
		w[i].i = i;   w[i].e = 0;
	}
	BFS2(r, w, n, t);		// 计算结点高度
	w[s].e = INF;			// 源点无限多的水
	w[s].h++;				// 源点无限高, 提高 1, 成为最高的
	q.push(&w[s]);
	while (!q.empty())
	{
		tmp = q.top();   q.pop();
		if (f) f = 0;			// 第一次允许源点向周围推进
		else if (tmp->i == t || tmp->i == s) continue;
		for (i=0; i<n; i++)		// 向邻接点推进
		{						// 比 i 点高
			if (r[tmp->i][i] != 0 && tmp->h > w[i].h)
			{
				PUSH2(r, w, tmp->i, i);
				q.push(&w[i]);
			}
		}
		if (tmp->i != s && tmp->e != 0)	// 不是源点, 且还有余量, 则重标记
		{
			// 找一个最低的邻接点
			min = INF;
			for (i=0; i<n; i++)
				if (r[tmp->i][i] != 0 && w[i].h < min) 
					min = w[i].h;
			tmp->h = min + 1;
			q.push(tmp);	// 再次入队
		}
	}
	return w[t].e;
}

int main(void)
{
	int n;
	int matrix[NUM][NUM];		// 邻接矩阵
	int a, b, c, d, e;

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
	d = maxflow(matrix, n, a, b);		// 普通队列处理
	e = maxflow2(matrix, n, a, b);		// 优先队列处理
	printf("\n最大流: %d\n最大流: %d\n", d, e);
	return 0;
}
