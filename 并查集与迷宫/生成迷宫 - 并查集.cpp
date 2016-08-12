
// 并查集生成迷宫
// 算法: 
// 1. 当起点和终点不在一个集合时, 做下面的步骤
// 2. 随机选择一条合适的边
// 3. 看左右或上下的两个点是不是一个集合的
// 4. 如果不是就拆墙, 合并两边的点到一个集合, 直到 1 成立

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define CHANGE(n) (2 * (n) - 1)
#define DOUBLE(n) (2 * (n))
#define A ' '
#define B 'X'

// 坐标点
typedef struct 
{
	int x, y;
}point;

// 迷宫的点结构
typedef struct
{
	char pass;		// 当前通或不通
	point parent;	// 双亲结点
}node;

// 迷宫结构
typedef struct
{
	node p[40][78];	// 也可动态创建 point *base;
	int x, y;			// 大小 (x * y)
	int sx, sy;			// 起点 (sx, sy)
	int ex, ey;			// 终点 (ex, ey)
}map;


// 寻找 (x, y) 的根, 并压缩路径
inline point find_r(map *m, int x, int y)
{
	point r;		// 根
	point tmp, tmp2;

	r.x = x;   r.y = y;		// 查找根结点
	while (m->p[r.x][r.y].parent.x != r.x 
		|| m->p[r.x][r.y].parent.y != r.y)
	{
		tmp.x = m->p[r.x][r.y].parent.x;
		tmp.y = m->p[r.x][r.y].parent.y;
		r = tmp;
	}
	// 再从叶子走到根, 压缩路径, 这里用非递归压缩
	// 因为迷宫太大, 递归工作栈容易溢出
	tmp.x = x;   tmp.y = y;
	while (m->p[tmp.x][tmp.y].parent.x != r.x 
		|| m->p[tmp.x][tmp.y].parent.y != r.y)
	{
		// tmp2 保存双亲信息
		tmp2.x = m->p[tmp.x][tmp.y].parent.x;
		tmp2.y = m->p[tmp.x][tmp.y].parent.y;
		m->p[tmp.x][tmp.y].parent.x = r.x;
		m->p[tmp.x][tmp.y].parent.y = r.y;
		tmp = tmp2;
	}
	return r;
}

// 合并 (a, b), (c, d) 所在的集合
void merge(map *m, int a, int b, int c, int d)
{
	point r1, r2;

	r1 = find_r(m, a, b);
	r2 = find_r(m, c, d);
	if (r1.x != r2.x || r1.y != r2.y)
	{
		m->p[c][d].parent.x = r1.x;
		m->p[c][d].parent.y = r1.y;
	}	// 合并到 (a, b) 的集合
}

void create(map *m)		// 生成迷宫到 m 
{
	int i, j;
	int x, y;
	point start, end;
	point s2, e2;		// 记录点(a,b) (c,d) 的根
	int a, b, c, d;		// 点(a,b), 点(c,d)

	// 坐标翻倍
	m->x = CHANGE(m->x);   m->y = CHANGE(m->y);
	m->sx = DOUBLE(m->sx - 1);  m->sy = DOUBLE(m->sy - 1);	
	m->ex = DOUBLE(m->ex - 1);  m->ey = DOUBLE(m->ey - 1);
	for (i=0; i<m->x; i++)
	{
		for (j=0; j<m->y; j++)
		{
			if (!(i % 2 || j % 2)) m->p[i][j].pass = A;
			else m->p[i][j].pass = B;
			m->p[i][j].parent.x = i;
			m->p[i][j].parent.y = j;
			// 开始没有双亲, 初始化并查集
		}
	}
	srand(time(NULL));
	while (1)			// 开始生成
	{
		start = find_r(m, m->sx, m->sy);
		end = find_r(m, m->ex, m->ey);
		if (start.x == end.x && start.y == end.y) 
			break;		// 合并到一个集合了

		do {	// 当 x + y 为奇数时, 是可拆的边
			x = rand() % m->x;
			y = rand() % m->y;
		} while ((x + y) % 2 == 0);
		// 此时 (x, y) 是待拆的墙
		if (x % 2)		// 看上下结点
		{
			b = d = y;   a = x + 1;   c = x - 1;
		}
		else		// 看左右结点
		{
			a = c = x;   b = y + 1;  d = y - 1;	
		}
		s2 = find_r(m, a, b);
		e2 = find_r(m, c, d);
		// 两个点是不同的集合
		if (s2.x != e2.x || s2.y != e2.y) 
		{
			merge(m, a, b, c, d);	// 合并 (a,b), (c,d)
			m->p[x][y].pass = A;	// 拆墙
		}
	}
	// 难度设置, 可以再多拆几面墙...
	m->p[m->sx][m->sy].pass = '.';
	m->p[m->ex][m->ey].pass = '.';
}

void show(map *m)		// 输出迷宫
{
	int i, j;
	for (i=0; i<79; i++) putchar('_');
	putchar('\n');
	for (i=0; i<m->x; i++)
	{
		putchar('|');
		for (j=0; j<m->y; j++)
		{
			putchar(m->p[i][j].pass);
		}
		putchar('|'); putchar('\n');
	}
	for (i=0; i<79; i++) putchar('-');
}

int main(void)
{
	map m;		// 迷宫地图

	printf("请输入地图大小: ");		// 迷宫最大 (20 * 39)
	scanf("%d %d", &m.x, &m.y);
	printf("设置起点: ");
	scanf("%d %d", &m.sx, &m.sy);
	printf("设置终点: ");
	scanf("%d %d", &m.ex, &m.ey);
	if (m.x < 2 || m.y< 2 || m.x > 20 || m.y > 39) return 0;

	create(&m);		// 生成迷宫
	show(&m);		// 输出
	printf("\n");

	return 0;
}