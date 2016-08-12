
// 非递归深搜生成迷宫
// 算法: 把深搜的路径作为迷宫的通路

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define CHANGE(n) (2 * (n) - 1)
#define A ' '
#define B 'X'
#define NO -1

// 坐标点
typedef struct 
{
	int x, y;
}point;

// 迷宫结构
typedef struct
{
	char p[41][79];	// 也可动态创建 point *base;
	int x, y;			// 大小 (x * y)
//	int sx, sy;			// 起点 (sx, sy)
//	int ex, ey;			// 终点 (ex, ey)
}map;

int used[41][79];	// 访问标记数组

inline int ok(map *m, int x, int y)
{
	if (x > 0 
		&& x <= m->x 
		&& y > 0 
		&& y <= m->y 
		&& !used[x][y])
		return 1;
	else return 0;
}

void show(map *m)		// 输出迷宫
{
	int i, j;
	for (i=0; i<79; i++) putchar('_');
	putchar('\n');
	for (i=0; i<=m->x+1; i++)
	{
		putchar('|');
		for (j=0; j<=m->y+1; j++)
		{
			putchar(m->p[i][j]);
		}
		putchar('|'); putchar('\n');
	}
	for (i=0; i<79; i++) putchar('-');
}

void create(map *m)		// 生成迷宫到 m 
{
	int i, j;
	point tmp;
	point stack[10000];
	int top = 0;
	int dir[4][2] = {0,-1, 0,+1, -1,0, +1,0};
	point re[4];		// 记录四周坐标
	int flag, r;
	int tx, ty;

	// 坐标翻倍
	m->x = CHANGE(m->x);   m->y = CHANGE(m->y);
	m->x++;   m->y++;
	for (i=0; i<=m->x; i++)		// 初始化地图
	{
		for (j=0; j<=m->y; j++)
		{
			m->p[i][j] = B;
			used[i][j] = 0;
		}
	}
	m->x--;   m->y--;
	// 起点入栈
	stack[top].x = 1;   stack[top].y = 1;
	top++;
	used[1][1] = 1;		// 标记访问
	srand(time(NULL));
	while (top != 0)
	{
		tmp = stack[top-1];		// 获得栈顶
		flag = 0;
		for (i=0; i<4; i++)		// 看看四周能不能走
		{
			tx = tmp.x+dir[i][0];
			ty = tmp.y+dir[i][1];
			if (ok(m, tx, ty)		// 剪枝过程
				&& (( !used[tx+1][ty] && !used[tx-1][ty] 
				&& (used[tx][ty+1]!=1 || used[tx][ty-1]!=1)) 
				|| (!used[tx][ty+1] && !used[tx][ty-1])) 
				&& (used[tx+1][ty]!=1 || used[tx-1][ty]!=1))
			{
				// 记录下能走的方向
				re[i].x = tx;
				re[i].y = ty;
				flag = 1;
			}
			else re[i].x = re[i].y = NO;
		}
		if (!flag) top--;		// 四周不能走了
		else 
		{
			while (1)		// 随机产生一个方向
			{
				r = rand() % 4;
				if (re[r].x != NO) break;
			}
			m->p[re[r].x][re[r].y] = A;		// 打通, 记录
			stack[top].x = re[r].x;   stack[top].y = re[r].y;
			top++;
			used[re[r].x][re[r].y] = 1;

//			show(m);			// 这段代码可观察迷宫生成过程
//			system("cls");
		}
	}
	m->p[1][1] = '.';			// 标记起点, 终点
	m->p[m->x][m->y] = '.';
}


int main(void)
{
	map m;		// 迷宫地图

	printf("请输入地图大小: ");		// 迷宫最大 (20 * 39)
	scanf("%d %d", &m.x, &m.y);
	if (m.x < 2 || m.y < 2 || m.x > 20 || m.y > 39) return 0;

	create(&m);		// 生成迷宫
	show(&m);		// 输出
	printf("\n");

	return 0;
}