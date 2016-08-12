
// 最优二叉查找树
// 以前构造其它动态查找表的时候, 有一个很重要的指标是平均查找长度 (ASL)
// ASL 越小越好, 计算 ASL 是以 假设所有结点被找到的概率均等 为前提的

// 而最优二叉查找树中, 每个结点被找到的概率不一定相等, 不能再用 ASL 这个指标
// 一味的把概率大的结点放到离根近的地方是不行的 (这是哈夫曼树的构造方法)
// 它同时还要满足二叉查找树的中序有序性质, 
// 所以引入一个新的指标 - 搜索代价 e[][], 或称查找总长度
// 搜索代价 (查找总长度) 要最小, 才是最优二叉查找树

#include <stdio.h>

#define NUM 30		// 最大结点数
#define INF 0xFFFFFF

// 搜索代价计算公式推导见 <算法导论>
float create_tree(int (*root)[NUM], float *node, float *q, int n)
{
	int i, j, k, s;
	float tmp;
	float e[NUM][NUM];		// 搜索代价
	float w[NUM][NUM];		// 概率和
	// e[i][j], 表示结点 i 到 j 的搜索代价
	// w[i][j], 表示结点 i 到 j 被搜到的概率和

	for (i=1; i<=n+1; i++)		// i 到 i-1 是 q[0]
		e[i][i-1] = w[i][i-1] = q[i-1];

	for (s=1; s<=n; s++)		// 包含 s 个元素的子树
	{
		for (i=1; i<=n-s+1; i++)		// i 为起点, n-s+1 去掉 s 后, 剩余的元素个数 
		{
			j = i + s - 1;			// j 为终点, j 与 i 相距 s 的距离
			e[i][j] = INF;
			w[i][j] = w[i][j-1] + node[j] + q[j];	// 由公式推出
			for (k=i; k<=j; k++)		// 枚举 i 到 j 的子根 k
			{
				tmp = e[i][k-1] + e[k+1][j] + w[i][j];	// 由公式推出
				if (tmp < e[i][j])
				{
					e[i][j] = tmp;
					root[i][j] = k;
				}
			}
		}
	}
	return e[1][n];		// 1 到 n 的最优解
}

int main(void)
{
	int root[NUM][NUM];		// 记录子根
	float node[NUM];		// 结点被搜到的概率
	float q[NUM];			// 搜不到的概率
	// root[i][j], 记录 i 到 j 的根
	// 例: node[3] = 0.25, 结点 3, 被搜到的概率 0.25
	float e;
	int n;

	n = 5;			// 5 个结点, 请确保 5 个点的概率和为 1
	node[1] = 0.15f;
	node[2] = 0.1f;
	node[3] = 0.05f;
	node[4] = 0.1f;
	node[5] = 0.2f;
	q[5] = q[1] = 0.1f;
	q[0] = q[2] = q[3] = q[4] = 0.05f;
	
	e = create_tree(root, node, q, n);
	printf("根是 %d\n", root[1][n]);
	printf("最小搜索代价 %.2f\n", e);
	
	return 0;
}