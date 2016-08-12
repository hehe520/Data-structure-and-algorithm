
// 查找有向图的强连通分量 Tarjan 算法
// 和 tarjan 算法求割点一样
// 用 num[] 记录深搜时结点的访问顺序
// 用 low[] 记录每个结点能回到深搜生成树的最小结点
// 如果 low[k] == count[k] 就说明 k 能回到 k
// 那么栈中 k 上的元素和 k 是一个强连通分量

#include <stdio.h>
#include <string.h>

#define NUM 60
#define MIN(a, b) ((a) > (b) ? (b) : (a))

int m[NUM][NUM];		// 邻接矩阵, 原图
int n;					// 顶点数
int num[NUM];			// 深搜访问顺序
int number;				// 标号
int low[NUM];			// 能到达最小的点
int used[NUM];			// 访问标记
int stack[NUM];			// 记录访问的结点
int top;
int in_the_stack[NUM];	// 标志在不在栈中
int count;				// 记录强连通个数


void DFS(int cur)
{
	int i;

	used[cur] = 1;
	low[cur] = num[cur] = number++;
	stack[top++] = cur;
	in_the_stack[cur] = 1;
	// 标记在不在栈中, 只有在栈中的点才是同一棵深搜生成树的

	for (i=0; i<n; i++)
	{
		if (m[cur][i])
		{
			if (!used[i])
			{
				DFS(i);		// 回溯时处理 low
				low[cur] = MIN(low[cur], low[i]);
			}
			else if (in_the_stack[i])
				low[cur] = MIN(low[cur], num[i]);
			// 此时 i 已经访问过, 但是 low[i] 可能没求出来
			// 因为在回溯的时候才求 low 的
			// 在栈中的 i 才是和 cur 在同一棵深搜生成树上的
			// 才有可能是同一个强连通, 防止搜到了其它子树上
		}
	}
	// 上面已经遍历完 cur 的所有子树了
	if (low[cur] == num[cur])		// 找到一个连通分量
	{
		// 如果 low[cur] < num[cur] 说明深搜树上面还有更大的连通分量
		// 只有 low[cur] == num[cur] 时, 说明 cur 刚好能回到自己
		// low[cur] > num[cur] 这种情况被上面的 == 排除掉了
		count++;
		printf("\n{");
		// 把 cur 以及 cur 之上的点全部出栈
		do
		{
			top--;
			printf("%d, ", stack[top]);
			in_the_stack[stack[top]] = 0;
		}
		while (top >= 0 && stack[top] != cur);
		printf("}");
	}
}

int Tarjan(void)
{
	int i;

	number = count = top = 0;
	for (i=0; i<n; i++) 
		in_the_stack[i] = used[i] = 0;
	for (i=0; i<n; i++)		// 访问
	{
		if (!used[i]) DFS(i);
	}
	return count;
}

int main(void)
{
	int i, j;
	int count;

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
	count = Tarjan();
	printf("\n强连通分量个数: %d\n", count);

	return 0;
}