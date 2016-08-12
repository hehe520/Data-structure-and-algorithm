
// 判断图是否存在环, 算法详见 <操作系统> 249 页
// 对每个点发起一个深搜, 如果能搜回自己说明存在环
// 顺便提一下, 数据库系统中判断用户是否循环授权, 也是检测环

#include <stdio.h>

#define NUM 60		// 最大顶点数

int m[NUM][NUM];		// 邻接矩阵
int used[NUM];			// 访问标记

void DFS(int n, int cur)	// 一次深搜
{
	int i;

	for (i=0; i<n; i++)
	{
		if (!used[i] && m[cur][i] != 0)
		{
			used[i] = 1;
			DFS(n, i);
		}
	}
}

// 有环返回 1, 无环返回 0
int juage(int n)
{
	int i, j;
	
	// 对每个点发起深搜
	for (i=0; i<n; i++)
	{
		for (j=0; j<n; j++) used[j] = 0;
		DFS(n, i);
		if (used[i] == 1) return 1;
		// 如果 i 点被访问了, 说明从 i 出发又回到了 i, 存在环
	}
	return 0;
}

int main(void)
{
	int n;
	int i, j;
	int a, b;

	printf("输入顶点个数: ");
	scanf("%d", &n);
	for (i=0; i<n; i++)
		for (j=0; j<n; j++) m[i][j] = 0;
	printf("输入边集: ");
	while (1)
	{
		scanf("%d %d", &a, &b);
		if (!(a || b)) break;
		m[a][b] = 1;
	}
	if (juage(n)) printf("\n存在环, 会死锁\n");
	else printf("\n无环, 不会死锁\n");

	return 0;
}