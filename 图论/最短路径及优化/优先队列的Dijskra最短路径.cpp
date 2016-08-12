
// dijskra 最短路径, 用优先队列优化
// 未优化 dijskra 算法, 每次都要遍历所有结点, 找一个最小的路径进行松弛
// 遍历 N 个结点时间 O(N), 相当于选择排序, 复杂度 O(N^2)

// 优先队列能在 O(1) 时间给出最小值, 但是插入要 O(log N) 的时间
// 相当于堆排序,  O(log N) + O(1) < O(N), 优化后, 复杂度为 O(N log N)

#include <cstdio>
#include <queue>
#include <vector>

using namespace std;

#define SIZE 30			// 最多 30 个点
#define INF 0xFFFFF
void show(int *pre, int end);		// 输出路径

typedef pair<int , int> p;		// 打包 (dis[i], i), 也可用结构体打包
struct cmp						// STL 的比较函数, 详细用法百度STL
{
	bool operator() (const p a, const p b)
	{ return a.first > b.first; }		// dis[i] 小的放前面
};

// 使用优先队列的 dijskra 最短路径, O(n log n)
int short_path_p(int (*m)[SIZE], int n, int start, int end)
{
	int dis[SIZE];		// 从 start 到各个点的最小值
	int used[SIZE];		// 访问标记
	int pre[SIZE];		// 记录前驱
	int i, j;
	priority_queue <p, vector<p>, cmp > q;
	// STL 优先队列 priority_queue, 详细用法百度STL
	
	for (i=0; i<n; i++) dis[i] = INF;
	for (i=0; i<n; i++) used[i] = 0;
	dis[start] = 0;
	pre[start] = -1;
	q.push(make_pair(dis[start], start));	// 把 (dis[start], start) 入队
	while (!q.empty())
	{
		// 从优先队列中拿个最小的
		p u = q.top();   q.pop();
		if (used[u.second]) continue;
		used[u.second] = 1;
		for (j=0; j<n; j++)		// 用 pos 松弛其他点
		{
			if (!used[j] && dis[u.second] + m[u.second][j] < dis[j])
			{
				dis[j] = dis[u.second] + m[u.second][j];
				pre[j] = u.second;
				q.push(make_pair(dis[j], j));	// 把 (dis[j], j) 入队
				// 可能会出现重复的 (dis[j], j) 但是 dis[j] 小的会在前面
			}
		}
	}
	printf("\n起点");
	show(pre, end);
	return dis[end];
}

// 普通做法: dijskra 最短路径, O(n^2)
int short_path(int (*m)[SIZE], int n, int start, int end)
{
	int dis[SIZE];		// 从 start 到各个点的最小值
	int used[SIZE];		// 访问标记
	int pre[SIZE];		// 记录前驱
	int i, j, pos;
	int min;

	for (i=0; i<n; i++) dis[i] = INF;
	for (i=0; i<n; i++) used[i] = 0;
	dis[start] = 0;
	pre[start] = -1;
	for (i=0; i<n; i++)		// n 个点, 需要 n-1 条边, 循环 n-1 次
	{
		pos = min = INF;
		for (j=0; j<n; j++)		// 遍历所有结点, 找个最小的
		{
			if (!used[j] && dis[j] < min)
			{
				min = dis[j];
				pos = j;
			}
		}
		if (pos == INF) return -1;		// 图不连通
		used[pos] = 1;
		for (j=0; j<n; j++)		// 用 pos 松弛其他点
		{
			if (!used[j] && dis[pos] + m[pos][j] < dis[j])
			{
				dis[j] = dis[pos] + m[pos][j];
				pre[j] = pos;
			}
		}
	}
	printf("\n起点");
	show(pre, end);
	return dis[end];
}

void show(int *pre, int end)	// 递归输出, 若路径过长用非递归的
{
	if (end == -1) return ;
	show(pre, pre[end]);
	printf(" -> %d", end);
}

int main(void)
{
	short int kind;			// 图的类型
	int m[SIZE][SIZE];		// 邻接矩阵
	int num;				// 顶点个数
	int a, b, c;		// m[a][b] = c
	int s, e;
	int sum;

	printf("输入一个图, 0.有向图,  1.无向图 :");
	scanf("%d", &kind);
	printf("顶点个数: ");
	scanf("%d", &num);
	printf("输入起点, 终点:");
	scanf("%d %d", &s, &e);
	for (a=0; a<num; a++)
		for (b=0; b<num; b++) m[a][b] = INF;
	for (a=0; a<num; a++) m[a][a] = 0;
	while (1)
	{
		scanf("%d %d %d", &a, &b, &c);
		if (!(a || b || c)) break;
		m[a][b] = c;
		if (kind) m[b][a] = c;
	}
//	sum = short_path(m, num, s, e);
	sum = short_path_p(m, num, s, e);
	printf("\n最小权值: %d\n", sum);

	return 0;
}
