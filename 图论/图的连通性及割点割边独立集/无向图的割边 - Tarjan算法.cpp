
// 求无向图的割边
// 割边: 去掉该边, 该图成为两个连通分支
// 和求割点一样, 用到 num[] 记录深搜顺序
// low[] 记录能够回到的最小结点
// 对于一条边 (a, b) 如果发现 low[b] > num[a] (a, b) 即是割边
// 与求割点类比, 求割点中 low[b] >= num[a] 才说明 a 是割点
// 因为深搜生成树上 b 能回到 a 把 a 拿掉, b 这棵子树就掉了
// 而求割边中, 如果 b 还能回到 a 那么把 (a, b) 拿掉. b 还有边与 a 相连
// 只有 b 回不到 a 了, 即 low[b] > num[a] (a, b) 才是割边

#include <stdio.h>

#define NUM 60
#define MIN(a, b) ((a) > (b) ? (b) : (a))

int m[NUM][NUM];		// 邻接矩阵
int n;
int used[NUM];			// 访问标记
int num[NUM];			// 访问顺序
int number;
int low[NUM];			// 最小回点
int count;

// 查找单个分支中的割边, 输出到屏幕, 也可保存到数组中
void DFS(int cur)
{
	int i;

	used[cur] = 1;
	num[cur] = low[cur] = number++;
	for (i=0; i<n; i++)
	{
		if (m[cur][i])
		{
			if (!used[i])
			{
				m[i][cur] = 0;			// 消除回边
				// 防止深搜树上的下一层结点的 low 更新到 cur
				DFS(i);
				low[cur] = MIN(low[cur], low[i]);
				if (low[i] > num[cur])		// 找到一条割边
				{
					count++;
					printf("(%d, %d), ", cur, i);
				}
			}
			else low[cur] = MIN(low[cur], num[i]);
		}
	}
}

int edge(void)		// 求割边
{
	int i;

	number = count = 0;
	for (i=0; i<n; i++) used[i] = 0;
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
		m[j][i] = 1;
	}
	printf("\n割边: ");
	count = edge();
	printf("\n割边个数: %d\n", count);

	return 0;
}