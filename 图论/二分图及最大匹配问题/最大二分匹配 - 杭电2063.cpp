
// 杭电 2063 题 - 过山车

#include <stdio.h>

int k, m, n;		// k 组数据, m 个女生, n 个男生
int map[510][510];	// 邻接矩阵记录边
int link[510];		// link[女生], 表示匹配的男生
int used[510];		// 男生访问标记

// 寻找增广路
int find_path(int g)
{
	int i;

	for (i=0; i<=n; i++)	// 遍历男生
	{
		// 如果女生 g 到男生 i 有路
		if (!used[i] && map[g][i])
		{
			used[i] = 1;
			if (link[i] == -1 || find_path(link[i]))
			{
				link[i] = g;
				return 1;
			}
		}
	}
	return 0;
}

int main(void)
{
	int i, j;
	int g, b;
	int count;

	while (scanf("%d", &k) != EOF && k)
	{
		scanf("%d %d", &m, &n);
		for (i=0; i<=m; i++)
			for (j=0; j<=n; j++) map[i][j] = 0;
		while (k--)
		{
			scanf("%d %d", &g, &b);
			map[g][b] = 1;
			// 从女生集合出发, 匹配男生
			// 所以不用记录 map[b][g] = 1 的反向边
			// 所以男生集合要设置访问数组
		}
		count = 0;
		for (j=0; j<=n; j++) link[j] = -1;
		for (i=1; i<=m; i++)	// 从女生集合出发
		{
			for (j=0; j<=n; j++) used[j] = 0;
			if (find_path(i)) count++;
		}
		printf("%d\n", count);
	}
	return 0;
}
