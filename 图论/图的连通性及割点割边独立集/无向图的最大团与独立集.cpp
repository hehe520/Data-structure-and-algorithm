
// 无向图的最大团与独立集
// 名词解释: (严格定义见 <离散数学>)
// 最大团即: 最大完全子图, 该图中任意两个有直接到达的边
// 独立集即: 补图, 某图中的一个点集, 该集合中的点, 任意两点都是没有直达边的

// 深搜查找最大团, 最大团的点集存储在 stack[] 中
// 剪枝: 只有当某点, 与栈中全部的点有直达边, 才可以入栈
// 一个图中剔除了最大团的点集, 剩下的就是独立集了
// 因为如果独立集中还有点与其它点相连, 那么将会被搜到, 于是成为最大团

#include <stdio.h>

#define NUM 60

int m[NUM][NUM];	// 邻接矩阵
int n;
int used[NUM];
int max;			// max 记录最大团的顶点数
int stack[200];		// 记录最大团的子结构
int top;
int map[NUM];		// 临时用

inline int ok(int cur)	// 剪枝
{
	int i;

	for (i=0; i<top; i++)
	{
		if (m[stack[i]][cur] == 0) return 0;
	}
	return 1;
}

void DFS1(int cur)		// 求最大团顶点数
{
	int i;

	for (i=0; i<n; i++)
	{
		if (!used[i] && m[cur][i] && ok(i))
		{
			used[i] = i;
			stack[top++] = i;
			if (top > max) max = top;
			DFS1(i);		// 继续递归
			top--;
			used[i] = 0;
		}
	}
}

void max_graph(void)
{
	int i;

	for (i=0; i<n; i++) used[i] = 0;
	max = -1;
	top = 0;
	DFS1(0);
	printf("\n最大团顶点个数: %d\n", max);
	printf("其中一个方案: \n");
	for (i=0; i<n; i++) map[i] = -1;
	printf("最大团: {");
	for (i=0; i<max; i++)
	{
		printf("%d, ", stack[i]);
		map[stack[i]]++;
	}
	printf("}, 独立集: {");
	for (i=0; i<n; i++)
		if (map[i] == -1) printf("%d, ", i);
	printf("}\n");
}

int main(void)
{
	int i, j;

	printf("输入顶点个数: ");
	scanf("%d", &n);
	for (i=0; i<n; i++)
		for (j=0; j<n; j++) m[i][j] = 0;
	while (1)
	{
		scanf("%d %d", &i, &j);
		if (!(i || j)) break;
		m[i][j] = 1;
		m[j][i] = 1;	// 无向图
	}
	max_graph();
	return 0;
}
/*
5
0 3
3 2
2 1
1 4
4 0
0 1
0 2
0 0
*/