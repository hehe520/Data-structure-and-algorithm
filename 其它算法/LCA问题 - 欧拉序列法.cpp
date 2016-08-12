
// LCA 问题的 RMQ 转化, 在一个树的深搜过程中, 每个点都会被访问到两次
// 这样形成了一个欧拉序列, 假设两个点 a, b 的最近公共祖先是 c 
// 那么在深搜的时候, 按照顺序访问, 如果要从 a 转到 b 
// 那么必定会经过一次 c 节点, 再从 c 节点转到 b 所在的子树上
// 也就是说, c 节点是这个从 a 到 b 的所有经过的节点之中, 深度最小的一个
// 只需要在第一次出现 a 和第一次出现 b 的欧拉序列之间
// 去找深度最小的点, 即为其最近公共祖先, 用 RMQ 找区间最值

#include <stdio.h>
#include <math.h>

#define NUM 500

int tree[2 * NUM];		// 二叉树
int top_t;
int depth[NUM];			// 深度
int eular[NUM];			// 欧拉序列, 先序遍历访问结点的顺序
int top_e;
int first[NUM];			// 结点 i 在欧拉序列中首次出现的位置
int min[300][300];		// RMQ 记录最小值的下标

// d 递归深度, 获得欧拉序列, 深度
void init(int d)
{
	int tmp = tree[top_t];

	top_t++;
	first[tmp] = top_e;
	eular[top_e] = tmp;
	depth[top_e++] = d;		// 记录深度

	if (tree[top_t] != -1) 
	{
		init(d + 1);		// 左子树
		eular[top_e] = tmp;
		depth[top_e++] = d;
	}
	else top_t++;

	if (tree[top_t] != -1) 
	{
		init(d + 1);		// 右子树
		eular[top_e] = tmp;
		depth[top_e++] = d;
	}
	else top_t++;
}

void init_RMQ(void)		// 详见 "RMQ问题.cpp"
{
	int i, j;
	int n = top_e-1;
	int k = (int)(log(top_e) / log(2));

	for (i=1; i<=n; i++) min[i][0] = i;
	for (j=1; j <= n-(j<<1)+1; j++)
	{
		for (i=1; i+(j<<1)-1 <= n; i++)
		{
			if (depth[ min[i][j-1] ] < depth[ min[i+(1<<(j-1))][j-1] ])
				min[i][j] = min[i][j-1];
			else 
				min[i][j] = min[i+(1<<(j-1))][j-1];
		}
	}
}

// 返回深度最小的元素的下标
int RMQ(int a, int b)
{
	int k = (int)(log(b-a+1) / log(2));
	// k 不超过 b-a 的最大幂长

	if (depth[min[a][k]] < depth[min[b-(1<<k)+1][k]])
		return min[a][k];
	else
		return min[b-(1<<k)+1][k];
}

int LCA(int a, int b)		// 转化为 RMQ 问题
{
	if (first[a] < first[b])
		return eular[ RMQ(first[a], first[b]) ];
	else
		return eular[ RMQ(first[b], first[a]) ];
}

int main(void)
{
	int a, b;

	// 类似二叉树的输入方式, -1 表示空树, -2 表示输入结束
	// 测试数据: 0 1 3 -1 -1 4 5 -1 -1 6 -1 -1 2 7 -1 -1 8 -1 -1 -2
	printf("输入一棵树: ");
	top_t = 0;
	while (1)
	{
		scanf("%d", tree + top_t);
		if (tree[top_t] == -2) break;
		top_t++;
	}
	top_e = top_t = 0;
	init(0);			// 初始化欧拉序列, 深度
	init_RMQ();			// 根据深度, 初始化 min
	printf("LCA 查询:\n");
	while (1)
	{
		scanf("%d %d", &a, &b);
		printf("最近公共祖先: %d\n", LCA(a, b));
	}
	return 0;
}