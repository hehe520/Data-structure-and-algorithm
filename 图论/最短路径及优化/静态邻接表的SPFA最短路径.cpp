
#include <stdio.h>
#include <queue>

#define NUM 60
#define INF 0xFFFFFF

typedef struct
{
	int i;		// 可达结点
	int v;		// 权值
}node;

int n;
node m[NUM][NUM];		// 静态邻接表
int top[NUM];
// m[k][0] 0 号单元表示 k 顶点的出度数
// m[a][..].i = c  表示 a 到 b 的权值是 c
// m[a][..].v = c  表示 a 到 b 的权值是 v

int SPFA(int s, int t, int *pre, int *value)
{
	std::queue<int> q;
	int used[NUM];
	int dis[NUM];		// 最短距离
	int i, tmp;
	int a, b, v;
	int count[NUM];		// 入队次数
	// n 个点中, 其中一个点与剩下的 n-1 个点至多有 n-1 条边
	// 所以至多入队 n-1 次, 如果超过了 n-1 说明存在负环
	// 每绕一次负环, dis[] 就更小, 最终造成死循环

	for (i=0; i<n; i++)
	{
		dis[i] = INF;
		used[i] = 0;
		count[i] = 0;
	}
	dis[s] = 0;
	q.push(s);
	used[s] = 1;
	count[s]++;
	while (!q.empty())
	{
		tmp = q.front();
		q.pop();
		for (i=0; i<top[tmp]; i++)
		{
			a = tmp;
			b = m[tmp][i].i;
			v = m[tmp][i].v;
			if (dis[a] + v < dis[b])
			{
				dis[b] = dis[a] + v;
				pre[b] = a;
				// 如果被松弛的结点 b, 没有在队列中
				// 入队, 因为让 b 去松弛其邻接点
				if (!used[b])
				{
					q.push(b);
					used[b] = 1;
					count[b]++;
					if (count[b] == n) return 0;
				}
			}
		}
		used[tmp] = 0;
	}
	*value = dis[t];
	return 1;
}

int main(void)
{
	int i;
	int a, b, c;		// (a, b) = c
	int pre[NUM];		// 记录前驱

	printf("输入顶点个数: ");
	scanf("%d", &n);
	for (i=0; i<n; i++)
	{
		top[i] = 0;
		pre[i] = -1;
	}
	while (1)
	{
		scanf("%d %d %d", &a, &b, &c);
		if (!(a || b || c)) break;
		m[a][top[a]].i = b;
		m[a][top[a]].v = c;
		top[a]++;
	}
	printf("输入起点, 终点: ");
	scanf("%d %d", &a, &b);
	if (SPFA(a, b, pre, &c))
	{
		printf("\n最短路径长: %d\n路径: ", c);
		c = b;
		while (c != -1)
		{
			printf("%d <- ", c);
			c = pre[c];
		}
		printf("起点\n");
	}
	else printf("\n存在负环!\n");

	return 0;
}
/*   两组测试数据, 一组有解, 一组有负环
5
0 1 6
0 2 7
1 2 8
1 3 5
3 1 -2
1 4 -4
2 3 -3
2 4 9
4 3 7
0 0 0
0 4

4
0 1 1
1 2 -2
2 3 -3
3 2 -4
2 4 1
0 0 0
0 2
*/