
/*			连连看 广搜 杭电 1175 题
输入数据有多组。每组数据的第一行有两个正整数n,m(0<n<=1000,0<m<1000)，
分别表示棋盘的行数与列数。在接下来的n行中，每行有m个非负整数描述棋盘的方格分布。
0表示这个位置没有棋子，正整数表示棋子的类型。接下来的一行是一个正整数q(0<q<50)，
表示下面有q次询问。在接下来的q行里，每行有四个正整数x1,y1,x2,y2,
表示询问第x1行y1列的棋子与第x2行y2列的棋子能不能消去。n=0,m=0时，输入结束。
注意：询问之间无先后关系，都是针对当前状态的！

Sample Input
3 4
1 2 3 4
0 0 0 0
4 3 2 1
4
1 1 3 4
1 1 2 4
1 1 3 3
2 1 2 4
3 4
0 1 4 3
0 2 4 1
0 0 0 0
2
1 1 2 4
1 3 2 3
0 0

Sample Output
YES
NO
NO
NO
NO
YES
*/

#include <stdio.h>
#include <math.h>
#include <queue>

int n, m;
int map[1003][1003];
int used[1003][1003];

int dir[4][2] = {0, 1, 0, -1, -1, 0, 1, 0};

typedef struct		// 广搜结点
{
	short int x, y;
	short int prex, prey;	// 记录前驱结点
	short int times;		// 记录拐弯次数
}node;

inline int ok(node *cur)		// 越界检查
{
	if (cur->x < 0 || cur->x >= n 
		|| cur->y < 0 || cur->y >= m) return 0;
	return 1;
}

// 两点距离
inline double dist(int x1, int y1, int x2, int y2)
{
	return sqrt((double)((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)));
}

int BFS(int x1, int y1, int x2, int y2)
{
	std::queue<node> q;
	int key1 = map[x1][y1];
	int i, j;
	node tmp, cur;
	double d;

	// 图案不同, 不能消去
	if (key1 == 0 || key1 != map[x2][y2]) return 0;
	for (i=0; i<n; i++)			// 初始化访问标记
		for (j=0; j<m; j++)
			used[i][j] = map[i][j];
	tmp.prex = tmp.x = x1;
	tmp.prey = tmp.y = y1;
	tmp.times = 0;
	q.push(tmp);
	used[x1][y1] = 1;
	while (!q.empty())
	{
		tmp = q.front();
		q.pop();
		for (i=0; i<4; i++)		// 分别扫描上0 下1 左2 右3
		{
			cur.x = tmp.x + dir[i][0];
			cur.y = tmp.y + dir[i][1];
			if (ok(&cur))		// 越界检查
			{
				// 计算前驱结点与当前结点的距离
				d = dist(cur.x, cur.y, tmp.prex, tmp.prey);
				// 有拐弯, 两个点的距离是根号2, 即 1.414
				if (d > 1.5 || d < 1.3)
					cur.times = tmp.times;
				else 
					cur.times = tmp.times + 1;
				// 拐弯次数不超过两次
				if (cur.times <= 2)
				{
					cur.prex = tmp.x;
					cur.prey = tmp.y;
					if (cur.x == x2 && cur.y == y2) return 1;
					// 没有访问过, 才能加入队列
					if (!used[cur.x][cur.y])
					{
						q.push(cur);
						used[cur.x][cur.y] = 1;
					}
				}
			}
		}
	}
	return 0;
}

int main(void)
{
	int i, j;
	int q;
	int x1, y1, x2, y2;

	while (scanf("%d %d", &n, &m), n || m)
	{
		for (i=0; i<n; i++)
			for (j=0; j<m; j++)
				scanf("%d", &(map[i][j]));
		scanf("%d", &q);
		for (i=0; i<q; i++)
		{
			scanf("%d %d %d %d", &x1, &y1, &x2, &y2);
			if (BFS(x1-1, y1-1, x2-1, y2-1)) 
				printf("YES\n");
			else 
				printf("NO\n");
		}
	}
	return 0;
}