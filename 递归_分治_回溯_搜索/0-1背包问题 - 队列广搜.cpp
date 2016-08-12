
// 0-1背包问题 - 队列广搜, 又称分支界限法, 和dijskra队列广搜算法一样
// 分支界限法与回溯法的区别:
// 回溯法搜索所有满足条件的解, 如N皇后的全部合法棋盘
// 分支界限法搜索一个满足条件的解, 如N皇后的一个合法棋盘
// 0-1背包问题的合法解只有一个, 即最大价值, 所以两种方法皆可

#include <cstdio>
#include <queue>

using namespace std;

#define NUM 100		// 最大背包数

typedef struct node		// 优先队列结点, 最大价值优先
{
	int v;		// value
	int w;		// weight
	int i;		// 第 i 件物品
	bool operator < (const node &n) const	// 常函数
	{ return (v < n.v); }
}node;

// 采用价值最大优先, 队头是当前背包最大的价值
// 从第 1 件物品开始, 拿或不拿, 直到队列为空
// 若当前重量超过背包容量, 进行剪枝
// 所以队列结点应包含三个元素, (价值, 重量, 第 i 件)
int BFS(int c, int n, int *w, int *v)
{
	priority_queue<node> q;		// stl 优先队列
	int max_v = 0;
	node tmp;
	
	tmp.v = 0;		// 一个无用的根入队, 便于进入循环
	tmp.w = 0;
	tmp.i = -1;
	q.push(tmp);
	while (!q.empty())
	{
		tmp = q.top();   q.pop();
		if (tmp.v > max_v) max_v = tmp.v;		// 记录最大值
		tmp.i++;				// 去到下一件物品
		if (tmp.i < n)			// 如果还有物品
		{
			q.push(tmp);		// 不拿, 右孩子入队
			// 如果没超背包容量, 继续拿, 左孩子入队
			if (tmp.w + w[tmp.i] <= c)
			{
				tmp.w += w[tmp.i];
				tmp.v += v[tmp.i];
				q.push(tmp);
			}
		}
	}
	return max_v;
}

int main(void)
{
	int weight[NUM];	// 重量
	int value[NUM];		// 价值
	int n, c;
	int max_value = 0;
	int i;

	printf("输入背包最大容量及物品总数: ");
	scanf("%d %d", &c, &n);
	printf("输入 %d 件物品: (重量 价值)\n", n);
	for (i=0; i<n; i++)
		scanf("%d %d", weight + i, value + i);
	max_value = BFS(c, n, weight, value);	// 广搜
	printf("\n最大价值: %d\n", max_value);
	return 0;
}
