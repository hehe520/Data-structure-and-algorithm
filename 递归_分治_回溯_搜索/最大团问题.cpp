
// 最大团问题, 查找一个最大完全子图, 输出点集

#include <stdio.h>

#define NUM 50

int n;				// 顶点数
int m[NUM][NUM];	// 邻接矩阵
int used[NUM];		// 访问标记
int stack[NUM];		// 记录点集
int top;
int max;
int result[NUM];

inline int ok(int cur)
{
	int i;

	for (i=0; i<top; i++)
	{
		if (m[cur][stack[i]] == 0) 
			return 0;
	}
	return 1;
}

// 回溯深搜
void DFS(int f)
{
	int i, j;

	for (i=0; i<n; i++)
	{
		if (!used[i] && ok(i))
		{
			used[i] = 1;
			stack[top++] = i;
			if (top > max)
			{
				max = top;
				for (j=0; j<top; j++)
					result[j] = stack[j];
			}
			DFS(f + 1);
			top--;		// 回溯恢复现场
			used[i] = 0;
		}
	}
}

int main(void)
{
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
		if (a == 0 && b == 0) break;
		m[a][b] = 1;
		m[b][a] = 1;
	}
	max = top = 0;
	DFS(0);
	printf("\n一个最大团点集:\n");
	for (i=0; i<max; i++)
		printf("%d, ", result[i]);
	putchar('\n');
	return 0;
}
/*		一组测试数据
5
0 1
0 3
0 4
3 4
1 4
1 2
4 3
0 0
*/