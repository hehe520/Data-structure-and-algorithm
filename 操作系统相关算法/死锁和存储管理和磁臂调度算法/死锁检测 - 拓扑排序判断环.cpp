
// 拓扑排序检测环
// 算法流程:
// 求得每个点入度 degree[]
// 寻找入度为 0 的点, 进行消去

#include <stdio.h>
#include <string.h>

#define NUM 50			// 最大顶点数

// 有环返回 1, 无环返回 0
int juage(int (*m)[NUM], int n)
{
	int degree[NUM];
	int i, j, tmp;
	int stack[300];
	int top = 0;

	// 计算每个点的入度
	memset(degree, 0, sizeof(degree));
	for (i=0; i<n; i++)
		for (j=0; j<n; j++)
			if (m[i][j] != 0) degree[j]++;

	// 找入度为 0, 入栈
	for (i=0; i<n; i++)
		if (degree[i] == 0)
			stack[top++] = i;

	// 栈中记录入度为 0 的点
	while (top != 0)
	{
		tmp = stack[--top];
		for (i=0; i<n; i++)
		{
			if (m[tmp][i] != 0)
			{
				degree[i]--;
				if (degree[i] == 0)
					stack[top++] = i;
			}
		}
	}		// 如果还存在有入度的点, 说明有环
	for (i=0; i<n; i++)
		if (degree[i] != 0) return 1;
	return 0;
}

int main(void)
{
	int m[NUM][NUM];		// 邻接矩阵
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
	if (juage(m, n)) printf("\n存在环, 会死锁\n");
	else printf("\n无环, 不会死锁\n");

	return 0;
}