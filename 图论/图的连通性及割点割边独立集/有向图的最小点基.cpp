
// 问题提出: 老师要通知所有学生一条消息, 老师有全部学生的电话
// 如果一个一个通知太麻烦, 有的学生会互相交换电话号码
// 即通知了 A 学生, A 学生又可以通知他认识的学生
// 则老师至少要通知多少个学生, 才能通知到所有学生
// 这就是最小点基数
// 算法: 将图中的极大强连通用 Tarjan 缩成点
// 查找入度为 0 缩点个数, 即是最小点基数

#include <stdio.h>

#define NUM 70		// 最大结点数
#define MIN(a, b) ((a) > (b) ? (b) : (a))

int m[NUM][NUM];		// 邻接矩阵
int n;
int stack[100];
int top;
int in_stack[NUM];		// 标记在不在栈中
int num[NUM];			// 深搜标号
int index;				// 标号
int low[NUM];			// 最小回点
int belong[NUM];		// 所属强连通
int count;				// 强连通个数
int degree[NUM];		// 统计缩点入度

// Tarjan 算法的深搜过程
void DFS(int cur)
{
	int i;

	low[cur] = num[cur] = index++;
	stack[top++] = cur;
	in_stack[cur] = 1;
	for (i=0; i<n; i++)
	{
		if (m[cur][i] != 0)
		{
			if (num[i] == -1)
			{
				DFS(i);
				low[cur] = MIN(low[cur], low[i]);
			}
			else if (in_stack[i])
				low[cur] = MIN(low[cur], num[i]);
		}
	}
	// 找到一个强连通, 把栈中 cur 以上的全部弹掉
	if (num[cur] == low[cur])
	{
		do 
		{
			i = stack[--top];
			in_stack[i] = 0;
			belong[i] = count;
		}
		while (stack[top] != cur);
		count++;
	}
}

// Tarjan 算法求最小点基
// 算法流程:
// 求得每个点所属的强连通
// 统计每个缩点的入度, 统计入度为 0 缩点
int Tarjan(void)
{
	int i, j;
	int a = 0;

	top = count = index = 0;
	for (i=0; i<n; i++)
	{
		belong[i] = num[i] = -1;
		in_stack[i] = degree[i] = 0;
	}
	for (i=0; i<n; i++)		// 统计所属强连通
	{
		if (num[i] == -1) DFS(i);
	}
	for (i=0; i<n; i++)		// 统计缩点入度
		for (j=0; j<n; j++)
			if (m[i][j] && belong[i] != belong[j]) 
				degree[ belong[j] ]++;
			// i, j 要是两个不同的强连通才算入度
	for (i=0; i<count; i++)
	{
		if (degree[i] == 0)		// 输出点基
		{
			a++;
			printf("{");
			for (j=0; j<n; j++)
				if (belong[j] == i)
					printf("%d, ", j);
			printf("}\n");
		}
	}
	return a;
}

int main(void)
{
	int i, j;
	int a;

	printf("输入顶点个数: ");
	scanf("%d", &n);
	for (i=0; i<n; i++)
		for (j=0; j<n; j++)
			m[i][j] = 0;
	while (1)
	{
		scanf("%d %d", &i, &j);
		if (i == 0 && j == 0) break;
		m[i][j] = 1;
	}
	printf("\n");
	a = Tarjan();
	printf("最小点基数: %d\n", a);

	return 0;
}
/*
8
0 1
1 4
4 0
1 5
4 5
5 6
6 5
2 3
3 2
2 6
3 7
6 7
0 0
*/