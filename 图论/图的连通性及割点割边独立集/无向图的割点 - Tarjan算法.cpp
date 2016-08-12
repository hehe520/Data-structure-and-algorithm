
// 查找无向图的割点
// 割点: 删除该点, 该图成为两个连通分支

// <数据结构> 上定义, 
// 1. 如果一个根不能访问到全部的结点, 即该根有多棵不相互连通的子树
// 那么把该根去掉, 不相通的子树成为多个连通分支
// 2. 如果一个根 k, 存在一棵子树, 不能回到比根还早的结点
// 那么该根是一个割点, 因为去掉该点, 某棵子树成为独立的分支

// 对原图发起一次深搜, 记录每个点的访问顺序 count[]
// 显然根是最先访问的, 所以根的 count[] 是最小的
// 显然从根到任意一个叶子的路径上, count[] 是递增的
// 所以任意一个结点 k 的子树的 count[] 不会比 k 小
// 再用 low[] 来表示每个结点能够回到 count[] 最小结点
// 如果 k 存在一棵子树 v , 它不能回到比 count[k] 还要小的地方
// 即 low[v] >= count[k], 那么把 k 拿掉, 这个子树成为独立的连通分支
// 即 k 是一个割点

#include <stdio.h>

#define NUM 60

int m[NUM][NUM];	// 邻接矩阵
int n;
int used[NUM];
int count[NUM];		// 深搜访问顺序
int num;			// 标号
int low[NUM];		// 可达的最小顶点
int map[NUM];		// map 中记录已经求得的割点

void DFS(int cur)		// 深搜, 打通单个连通分支
{
	int i;

	used[cur] = 1;
	low[cur] = count[cur] = num++;
	for (i=0; i<n; i++)
	{
		if (m[cur][i])
		{
			if (!used[i])
			{
				DFS(i);
				if (low[i] < low[cur])
					low[cur] = low[i];
				// i 是 cur 的子树
				// i 能回到的最小结点, 没有小于 cur
				// 此时 cur 就是一个割点, 记录入 map 中
				if (low[i] >= count[cur])
					map[cur] = 1;
			}
			else if (count[i] < low[cur])
				low[cur] = count[i];
			// 此时 i 已经访问过, 
			// 但可能 low[i] 还没求出来
			// 所以看能到达的 count 大小
		}
	}
}

void tarjan(void)		// tarjan 求割点
{
	int i;
	int root = 0;		// 随机选一个点做深搜生成树的根

	for (i=0; i<n; i++)		// 初始化
	{
		map[i] = count[i] = used[i] = 0;
		low[i] = 0XFFFFFF;
	}
	num = 0;
	used[root] = 1;			// 初始化根
	low[root] = count[root] = num++;

	for (i=0; i<n; i++)		// 访问 root 的一棵子树
	{
		if (m[root][i])
		{
			DFS(i);
			break;
		}
	}
	if (num < n)	// 没有访问完全部的点, 此时 root 是割点
	{
		map[root] = 1;
		for (i=0; i<n; i++)		// 把剩余的访问完
		{
			if (!used[i] && m[root][i]) DFS(i);
		}
	}
}

int main(void)
{
	int i, j;
	int zero;

	printf("输入顶点个数: ");
	scanf("%d", &n);
	for (i=0; i<n; i++)
		for (j=0; j<n; j++) m[i][j] = 0;
	printf("输入边集: ");
	while (1)
	{
		scanf("%d %d", &i, &j);
		if (!(i || j)) break;
		m[i][j] = 1;
		m[j][i] = 1;
	}
	tarjan();
	printf("\n割点集: ");
	for (zero=i=0; i<n; i++)
	{
		if (map[i]) printf("%d, ", i);
		zero += map[i];		// zero 多项 0 测试
	}
	if (zero == 0) printf("无割点");
	putchar('\n');

	return 0;
}