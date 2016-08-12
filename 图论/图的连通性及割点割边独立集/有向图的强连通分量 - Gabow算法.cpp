
// 有向图的强连通分量 Gabow 算法
// Tarjan 算法是用 low[i] == num[i] 来表示找到了一个极大强连通
// 而 Gabow 算法中用两个栈来找强连通
// 一个栈记录深搜访问顺序, 一个栈记录深搜顺序的同时, 还要消除子连通图

#include <stdio.h>
#include <string.h>

#define NUM 60

int m[NUM][NUM];		// 邻接矩阵
int n;
int used[NUM];
int stack1[100];		// 深搜栈
int stack2[100];		// 消除子连通栈
int top1, top2;
int map[NUM];			// 记录所属强连通
int num[NUM];			// 记录深搜编号
int index;				// 编号
int count;				// 强连通个数

void DFS(int cur)
{
	int i;

	stack1[top1++] = cur;
	stack2[top2++] = cur;
	num[cur] = index++;
	used[cur] = 1;
	for (i=0; i<n; i++)
	{
		if (m[cur][i] != 0)
		{
			if (!used[i]) DFS(i);
			else if (map[i] == -1)
			{
				// map[i] == -1 说明 i 还没归属到分量中
				// 那么 i 可能是某个极大强连通的子强连通
				// 把栈中 i 以上的点, 全部退掉
				while (num[stack2[top2 - 1]] > num[i]) top2--;
			}
		}
	}
	// 遍历完 cur 的邻接边, 看看 stack2 的栈顶是不是 cur
	// 如果是, 说明 cur 是某个强连通的根, 因为经过邻接边的消除
	// 仍不能把 cur 从 stack2 中消去
	if (stack2[top2 - 1] == cur)
	{
		top2--;			// 去掉 cur 根
		do
		{
			i = stack1[--top1];
			map[i] = count;
		}while (stack1[top1] != cur);
		count++;
	}
}

// 返回强连通个数
int gabow()
{
	int i;

	index = count = top1 = top2 = 0;
	memset(used, 0, sizeof(used));
	memset(map, -1, sizeof(map));
	for (i=0; i<n; i++)
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
	count = gabow();
	printf("\n强连通分量个数: %d\n", count);
	for (i=0; i<count; i++)
	{
		putchar('{');
		for (j=0; j<n; j++)
			if (map[j] == i) printf("%d, ", j);
		printf("}\n");
	}
	return 0;
}
