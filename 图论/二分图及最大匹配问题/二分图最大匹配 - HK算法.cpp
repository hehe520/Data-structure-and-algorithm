
// 二分图最大匹配 Hopcroft Karp算法
// 该算法与匈牙利算法的区别:
// 匈牙利算法每次找一条增广路进行翻转
// HK 算法每次找多条增广路进行翻转

// 算法流程:
// 先通过广搜找到多条增广路
// 如果广搜找不到增广路了, 就表示结束了
// 怎么记录多条增广路呢?
// 用到高度函数 h[] 只有 h[a] == h[b] + 1 表示 a b 是一条路上的

#include <stdio.h>
#include <queue>

#define NUM 80

int m[NUM][NUM];	// 静态邻接表
int top[NUM];		// 邻接表尾端
int x, y;			// 二分图 x, y 集合的个数
int xlink[NUM];		// x 集对应的匹配边
int ylink[NUM];		// y 集匹配边
int h[NUM];			// 高度函数

// 广搜找增广路
int BFS(void)
{
	int i, tmp, j;
	std::queue<int> q;
	int flag = 0;

	for (i=0; i<NUM; i++) h[i] = 0;
	for (i=0; i<x; i++)
	{
		if (xlink[i] == -1) q.push(i);
	}	// 未匹配的边入队
	while (!q.empty())
	{
		tmp = q.front();
		q.pop();
		for (i=0; i<top[tmp]; i++)		// 遍历 tmp 邻接点
		{
			j = m[tmp][i];
			if (h[j] == 0)
			{
				h[j] = h[tmp] + 1;
				if (ylink[j] == -1) flag = 1;	// 找到了一条增广路
				else 
				{
					// 如果 j [匹配过, 更新被匹配的点
					h[ylink[j]] = h[j] + 1;
					q.push(ylink[j]);
				}
			}
		}
	}
	return flag;
}

// 深搜查找增广路, 一条路上的点高度递增 1
int DFS(int cur)
{
	int i, j;

	for (i=0; i<top[cur]; i++)
	{
		j = m[cur][i];
		if (h[j] == h[cur] + 1)
		{
			h[j] = -1;		// 标记访问了
			if (ylink[j] == -1 || DFS(ylink[j]))
			{
				ylink[j] = cur;
				xlink[cur] = j;
				return 1;
			}
		}
	}
	return 0;
}

int HK(void)
{
	int count = 0;
	int i;

	for (i=0; i<NUM; i++)
		xlink[i] = ylink[i] = -1;
	while (BFS())
	{
		for (i=0; i<x; i++)		// 从 x 集出发
		{
			if (xlink[i] == -1 && DFS(i)) count++;
		}	// 未匹配的点尝试匹配
	}
	return count;
}

int main(void)
{
	int i, j;

	printf("输入 x, y 集合的顶点个数: ");
	scanf("%d %d", &x, &y);
	for (i=0; i<x; i++) top[i] = 0;
	// 输入边集的时候, 默认两个点属于两个集合
	while (1)
	{
		scanf("%d %d", &i, &j);
		if (!(i || j)) break;
		m[i][top[i]++] = j;
	}
	printf("\n最大匹配数: %d\n", HK());
	printf("匹配边: ");
	for (i=0; i<x; i++)
	{
		if (xlink[i] != -1)
			printf("(%d, %d), ", i, xlink[i]);
	}
	putchar('\n');
	return 0;
}
