
// 哈密顿回路
// 欧拉回路是每条边只能经过一次, 哈密顿回路是每个点只能经过一次
// 对原图发起一次深搜, 如果能走完全部结点即是一条哈密顿回路

#include <stdio.h>

#define NUM 60

int m[NUM][NUM];
int n;
int used[NUM];
int stack[100];
int top;

void DFS(int cur)		// 深搜
{
	int i;

	stack[top++] = cur;
	used[cur] = 1;
	for (i=0; i<n; i++)
	{
		if (!used[i] && m[cur][i])
		{
			DFS(i);
			break;
		}
	}
}

int judge()
{
	int i;

	top = 0;
	for (i=0; i<n; i++) used[i] = 0;
	DFS(0);
	// 如果没有访问完全部的点, 则不是
	for (i=0; i<n; i++)
		if (!used[i]) return 0;
	// 深搜最后的结点, 能不能形成回路
	if (m[stack[top-1]][0] == 0) return 0;
	return 1;
}

int main(void)
{
	int i, j;

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
	if (judge())
	{
		printf("\n哈密顿回路: ");
		for (i=0; i<top; i++)
			printf("%d -> ", stack[i]);
		printf("%d\n", stack[0]);
	}
	else printf("\n不是哈密顿回路!\n");

	return 0;
}
/*			两组测试数据
5
0 1
1 2
2 3
3 0
0 4
1 4
2 4
3 4
0 0

5
0 1
1 2
2 0
2 3
3 4
4 2
0 0
*/