
// Tarjan 算法是一种离线算法, 必须计算完全部的配对, 用 LCA 数组保存
// 然后才能 O(1) 的回答每个 LCA(a, b)

#include <stdio.h>

#define NUM 500

int tree[NUM];			// 二叉树
int n;					// 结点数
int num[NUM];			// 数组
int top;
int parent[NUM];		// 并查集双亲
int check[NUM];			// 类似访问标记
int ancestor[NUM];		// 集合的最近祖先
int LCA[NUM][NUM];		// 保存 LCA 结果

void create(int pos)		// 根据 num[] 建树 tree[]
{
	tree[pos] = num[top++];
	if (tree[pos] != -1)
	{
		n++;		// 记录结点数
		create(2 * pos);
		create(2 * pos + 1);
	}
}

void initialize(void)		// 根据 n 初始化并查集
{
	int i;

	for (i=0; i<n; i++) 
	{
		parent[i] = i;
		check[i] = 0;
	}
}

// 查找集合时, 同时压缩路径
int find_set(int cur)
{
	int stack[100];		// 注意栈大小
	int t = 0;

	while (parent[cur] != cur)
	{
		stack[t++] = cur;
		cur = parent[cur];
	}
	while (t != 0)
	{
		parent[stack[--t]] = cur;
	}
	return cur;
}

// 合并方向: 把 b 合并到 a 所在的集合
void union_set(int a, int b)
{
	int ra, rb;

	ra = find_set(a);
	rb = find_set(b);
	if (ra != rb)
	{
		parent[rb] = ra;
	}
}

// 深搜处理
void tarjan(int cur)
{
	int i;
	int child = 2 * cur;
	int set;

	set = find_set(tree[cur]);
	ancestor[set] = tree[cur];
	// 遍历子树, 二叉树最多两棵子树
	for (i=child; i<child+2; i++)
	{
		if (tree[i] > 0)		// tree[i] 结点不为空
		{
			tarjan(i);
			union_set(tree[cur], tree[i]);		// 合并子树
		}
	}
	// 将 tree[cur] 所在的集合的祖先标记为 tree[cur] 
	// 然后回答所有 LCA[ tree[cur] ][i] 的查询
	set = find_set(tree[cur]);
	ancestor[set] = tree[cur];
	check[tree[cur]] = 1;		// 回溯时, 标记检查过了
	for (i=0; i<n; i++)
	{
		if (check[i])		// i 结点名称
		{
			set = ancestor[find_set(i)];
			LCA[tree[cur]][i] = set;
			LCA[i][tree[cur]] = set;
		}
	}
}

int main(void)
{
	int a, b;

	// 二叉树的输入方式, -1 表示空树, -2 表示输入结束
	// 测试数据: 0 1 3 -1 -1 4 5 -1 -1 6 -1 -1 2 7 -1 -1 8 -1 -1 -2
	// 结点要按编号, 从 0 开始输入
	printf("输入一棵树: ");
	top = 0;
	while (1)
	{
		scanf("%d", num + top);
		if (num[top] == -2) break;
		top++;
	}
	n = top = 0;
	create(1);			// 初始化树
	initialize();		// 初始化并查集, 访问记录
	tarjan(1);			// 处理全部查询, 记录到 LCA[][]
	printf("LCA 查询:\n");
	while (1)
	{
		scanf("%d %d", &a, &b);
		printf("最近公共祖先: %d\n", LCA[a][b]);
	}
	return 0;
}