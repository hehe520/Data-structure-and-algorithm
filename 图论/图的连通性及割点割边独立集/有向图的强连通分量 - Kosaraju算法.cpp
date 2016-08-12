
// 查找有向图的强连通分量 Kosaraju 算法
// 名词解释: (严格定义见 <离散数学>)
// 强连通分量 : 该连通分量中任意两点之间都是可达的
// 反图: 对原图的边取反, 即对该图邻接矩阵转置

// 算法导论原话: 在原图中, a, b互为可达, 当且仅当反图中, a, b互为可达
// 如果原图中 a -> b 可达, 在反图中如果 a -> b 也可达
// 那么 a, b 属于同一个强连通分量中
// 即在原图反图中都 DFS(a), 两次都被访问到的点即是一个强连通
// 即从 a 出发不管顺着走, 还是逆着走, 都能回到 a, 那么经过的点是强连通的
// a 要是某棵子树的树根, 不然搜到的只是以 a 为根的分量, 不完整
// 所以要用 stack[] 记录子树树根
// 假设某个图搜到了四个子树: a, b, c, d
// 在原图中 a -> b 是无路的, 因为有路的话, a, b 也不会被拆成两棵树了
// 但是不能保证反图中 b -> a 无路, 假设原图中 b -> a 有路
// 那么在反图中 a -> b 就有路, 先搜索 a 的话, 会搜到 b 的树上, 这显然不对
// 所以要用 stack[] 记录 a, b, c, d, 然后逆序 d, c, b, a 来搜索

// Kosaraju 算法:
// 对原图发起一次深搜, 记录后序遍历的顺序
// 后序遍历中根位于最后, 想想二叉树的后序遍历
// 从后往前选择子根对反图进行深搜, 能找到的点就是一个强连通分量
// 例如子树根是 a, b, c, 那么对反图深搜时, 按照 c, b, a 的顺序

#include <stdio.h>
#include <string.h>

#define NUM 60

int m[NUM][NUM];		// 邻接矩阵, 原图
int fm[NUM][NUM];		// 邻接矩阵, 反图
int n;					// 顶点数
int used[NUM];			// 访问标记
int father[NUM];		// 记录所属连通分量
int stack[100];			// 记录后序遍历序列
int top;

void DFS1(int cur)		// 原图深搜, 记录 stack[]
{
	int i;

	used[cur] = 1;
	for (i=0; i<n; i++)
	{
		if (!used[i] && m[cur][i] != 0)
			DFS1(i);
	}
	stack[top++] = cur;		// 记录后序, 回溯时记录 cur
}

void DFS2(int cur, int root)	// 反图深搜, 记录所属连通分量
{
	int i;

	used[cur] = 1;
	father[cur] = root;		// 记录所属分量
	for (i=0; i<n; i++)
	{
		if (!used[i] && fm[cur][i] != 0)
			DFS2(i, root);
	}
}

// 返回最大连通分量个数, father[] 中储存所属分量, 有点并查集的味道
int kosaraju(void)
{
	int count = 0;	// 记录连通分量
	int i;

	memset(used, 0, sizeof(used));
	memset(father, 0, sizeof(father));
	top = 0;
	for (i=0; i<n; i++)		// 对原图深搜, 记录后序序列
	{
		if (!used[i]) DFS1(i);
	}
	memset(used, 0, sizeof(used));
	for (i=top-1; i>=0; i--)	// 从后往前搜子树的根
	{
		if (!used[stack[i]])
		{
			DFS2(stack[i], count);
			count++;
		}
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
			fm[i][j] = m[i][j] = 0;
	while (1)
	{
		scanf("%d %d", &i, &j);
		if (i == 0 && j == 0) break;
		m[i][j] = 1;		// 原图
		fm[j][i] = 1;		// 反图
	}
	count = kosaraju();
	printf("\n强连通分量个数: %d\n", count);
	for (i=0; i<count; i++)
	{
		printf("{");
		for (j=0; j<n; j++)
			if (father[j] == i) printf("%d, ", j);
		printf("}\n");
	}
	return 0;
}