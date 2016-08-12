
// 图的 m 点着色问题, 地图上相邻的国家不能是相同的颜色
// 给出一个图, m 种颜色, 求出所有着色方案

#include <stdio.h>

#define NUM 50
#define NO -1		// 无色

int n;		// n 个点
int m;		// m 个颜色
int k;		// 方案数
int c[NUM][NUM];
int used[NUM];
int color[NUM];		// 每个点的颜色

// 给 i 号结点上色, 上色成功返回 1, 失败返回 0
int ok(int cur)
{
	int i;
	int map[NUM];

	for (i=0; i<n; i++) map[i] = 0;
	// 遍历 i 的邻接边, 记录使用的颜色
	for (i=0; i<n; i++)
	{
		if (c[cur][i] != 0 && color[i] != NO)
			map[ color[i] ] = 1;
	}
	for (i=0; i<m; i++)
		if (map[i] == 0) break;		// 找到一个颜色 i 就 break
	if (i == m) return 0;
	color[cur] = i;			// 给 cur 上 i 号色

	return 1;
}

void DFS(int f)
{
	int i;

	if (f == n)
	{
		k++;
		printf("\n第 %d 组解:\n", k);
		for (i=0; i<n; i++)
			printf("%d 上 %d 号色\n", i, color[i]);
		return ;
	}
	for (i=0; i<n; i++)
	{
		if (!used[i] && ok(i))
		{
			used[i] = 1;
			DFS(f+1);
			color[i] = NO;	// 回溯清空颜色
			used[i] = 0;
		}
	}
}

void initiaize(int a)	// 初始化工作
{
	int i, j;

	for (i=0; i<n; i++)
	{
		for (j=0; j<n; j++) c[i][j] = 0;
		used[i] = 0;
		color[i] = NO;
	}
	k = 0;
}

int main(void)
{
	int a, b;

	printf("输入顶点个数: ");
	scanf("%d", &n);
	initiaize(n);			// 初始化工作
	printf("输入边集: ");
	while (1)
	{
		scanf("%d %d", &a, &b);
		if (a == 0 && b == 0) break;
		c[a][b] = 1;
		c[b][a] = 1;
	}
	printf("输入颜色个数: ");
	scanf("%d", &m);
	DFS(0);
	if (k == 0) printf("上色失败, 颜色种类太少\n");
	return 0;
}
/*		两组测试数据
5
0 1
0 2
0 3
1 2
1 3
1 4
2 3
3 4
0 0
4

3
0 1
1 2
2 0
0 0
2
*/