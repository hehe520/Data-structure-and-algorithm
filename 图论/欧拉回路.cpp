
// 判断欧拉回路, 如果有, 输出一条欧拉回路
// 欧拉通路: 所有的边只能经过一次, 走完所有边
// 欧拉回路: 所有的边只能经过一次, 走完所有边, 且回到原点
// 详细定义见 <离散数学>

#include <stdio.h>

#define NUM 60

int type;		// 标志图的类型
int n;
int m[NUM][NUM];
int degree[NUM];		// 无向图的度数
int used[NUM];
int in[NUM], out[NUM];	// 有向图的入度出度
int stack[100];			// 记录路径
int top;
int e;			// 边数

void DFS(int cur)
{
	int i;

	for (i=0; i<n; i++)
	{
		if (!used[i] && m[cur][i])
		{
			used[i] = 1;
			DFS(i);
		}
	}
}

// 判断是否存在欧拉回路:
// 无向图中: 连通图且所有顶点度数为偶数
// 有向图中: 连通图且所有顶点的入度等于出度
int judge(void)
{
	int i;

	// 深搜检查连通性
	for (i=0; i<n; i++) used[i] = 0;
	DFS(0);
	for (i=0; i<n; i++) 
		if (!used[i]) return 0;

	if (type)		// 有向图, 入度等于出度
	{
		for (i=0; i<n; i++)
			if (in[i] != out[i]) return 0;
	}
	else		// 无向图, 度数为偶数
	{
		for (i=0; i<n; i++)
			if (degree[i] % 2) return 0;
	}
	return 1;
}

// 有向图的欧拉回路, 在 cur 点, 从 pos 点开始搜
void DFS1(int cur, int pos)
{
	int i, a, b;

	stack[top++] = cur;
	for (i=pos; i<n; i++)
	{
		if (m[cur][i] != 0)
		{
			m[cur][i] = 0;
			out[cur]--;
			in[i]--;
			DFS1(i, 0);
			break;
		}
	}
	// 走投无路, 而且还有边的时候, 退回一步
	if (i == n && top < e)
	{
		b = stack[--top];
		a = stack[--top];
		m[a][b] = 1;		// 恢复边
		out[a]++;
		in[b]++;
		DFS1(a, b + 1);		// 从下一处开始搜
	}
}

// 无向图的欧拉回路, cur 点, 从 pos 点开始搜
void DFS2(int cur, int pos)
{
	int i, a, b;

	stack[top++] = cur;
	for (i=pos; i<n; i++)
	{
		if (m[cur][i] != 0)
		{
			m[i][cur] = 0;
			m[cur][i] = 0;
			degree[cur]--;
			degree[i]--;
			DFS2(i, 0);
			break;
		}
	}
	if (i == n && top < e)		// 退回一步
	{
		b = stack[--top];
		a = stack[--top];
		m[a][b] = 1;
		m[b][a] = 1;
		degree[a]++;
		degree[b]++;
		DFS2(a, b + 1);		// 从下一处开始搜
	}
}

int main(void)
{
	int i;
	int a, b;

	printf("0, 无向图    1, 有向图 : ");
	scanf("%d", &type);
	printf("输入顶点个数: ");
	scanf("%d", &n);
	for (i=0; i<n; i++)
	{
		in[i] = out[i] = 0;
		degree[i] = 0;
	}
	e = top = 0;
	printf("输入边集: ");
	while (1)
	{
		scanf("%d %d", &a, &b);
		if (!(a || b)) break;
		m[a][b] = 1;
		in[b]++;		// 顺便统计度数
		out[a]++;
		e++;
		if (!type)		// 如果是无向图
		{
			m[b][a] = 1;
			degree[a]++;
			degree[b]++;
		}
	}
	if (judge())
	{
		// 输出一条欧拉回路
		printf("\n一条欧拉回路: ");
		if (type) 
			DFS1(0, 0);
		else 
			DFS2(0, 0);
		for (i=0; i<top; i++)
		{
			printf("%d", stack[i]);
			if (i + 1 != top) printf(" -> ");
		}
		putchar('\n');
	}
	else printf("\n不存在欧拉回路!\n");

	return 0;
}