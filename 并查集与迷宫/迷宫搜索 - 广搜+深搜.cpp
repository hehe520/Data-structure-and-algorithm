// 迷宫搜索, 广搜，深搜
// 要点，对于访问过的点要进行标记，不然会重复访问的
// 文件最后是详细的算法描述

#include <stdio.h>
#include <stdlib.h>

#define MAXSIZE 21
#define USED 2		// 经过的路径
#define INF -1		// -1 没有前驱结点

// 坐标点
typedef struct
{
	short int x;
	short int y;
	short int quan;		// 该点的权值
	short int used;		// 使用标记
}point;

// 路径记录栈
typedef struct
{
	point base[200];
	int top;
}stack;

// 四个方向增量，上         右         下        左
point dir[4] = {-1,0,0,0,  0,1,0,0,  1,0,0,0,  0,-1,0,0};

// 输出迷宫
void showmap(point map[MAXSIZE][MAXSIZE], int m, int n)
{
	int i, j;
	for (i=0; i<m; i++)
	{
		for (j=0; j<n; j++)
			if (map[i][j].quan == 1) printf("%3c", '.');
			else if (map[i][j].quan == 0) printf("%3c", '*');
			else printf("%3c", 'O');
		printf("\n");
	}
}

// 初始化访问标记
void clear(point map[MAXSIZE][MAXSIZE], int m, int n)
{
	int i, j;
	for (i=0; i<m; i++)
		for (j=0; j<n; j++)
			map[i][j].used = 0;
}

// 初始化地图的权值
void init_quan(point map[MAXSIZE][MAXSIZE], int m, int n)
{
	int i, j;
	for (i=0; i<m; i++)
		for (j=0; j<n; j++)
			if (map[i][j].quan == USED) 
				map[i][j].quan = 1;
}

// 输入迷宫
void input(point map[MAXSIZE][MAXSIZE], int m, int n)
{
	int i, j;
	for (i=0; i<m; i++)
	{
		for (j=0; j<n; j++)
			scanf("%1d", &map[i][j].quan);
		scanf("%c");	// 吸掉一个回车键
	}
	// 初始化使用标记
	clear(map, m, n);
}

// 检查合法
inline int check(int m, int n)
{
	if (m <= 0 
		|| m > MAXSIZE - 1 
		|| n <= 0 
		|| n > MAXSIZE - 1)
	{
		printf("错误：迷宫最大不能超过 20*20 !\n");
		return 0;
	}
	else return 1;
}

// 判断当前点可不可以走
int find(point map[MAXSIZE][MAXSIZE], 
		 int cur_x, int cur_y, int m, int n)
{
	if (cur_x >= 0 
		&& cur_y >= 0 
		&& cur_x< m 
		&& cur_y < n 
		&& map[cur_x][cur_y].quan
		&& !map[cur_x][cur_y].used) 
		return 1;
	return 0;
}


// --------------------广搜算法要点------------------------
// 1. 用一个二维数组，记录路径，当前点是从哪个点过来的
// 2. 对于访问过的结点要进行标记，防止重复访问，
// BFS(地图，m行, n列, 起点，终点);
int BFS(point map[MAXSIZE][MAXSIZE], 
		 int m, 
		 int n,
		 point *s, 
		 point *e,
		 point p[MAXSIZE][MAXSIZE])
{
	point queue[100];
	int rear = 0;
	int front = 0;
	int i;
	point back;		// 即将出队的点
	point in;		// 即将入队的点
	int flag = 0;

	// 清空访问标记
	clear(map, m, n);
	// 起点入队
	queue[rear] = *s;
	rear = (rear + 1) % 100;
	map[s->x][s->y].used = 1;	// 标记使用了
	// 记录起点路径, INF 表示没有前驱点了
	p[s->x][s->y].x = INF;
	p[s->x][s->y].y = INF;

	while (rear != front)
	{
		back = queue[front];		// 出队
		front = (front + 1) % 100;
		// 判断到终点没有
		if (back.x == e->x && back.y == e->y)
		{
			flag = 1;
			break;
		}
		// 如果没到，把相邻的点入队,同时记录路径
		// 向四个方向搜索
		for (i=0; i<4; i++)
		{
			in.x = back.x + dir[i].x;
			in.y = back.y + dir[i].y;

			if (find(map, in.x, in.y, m, n))
			{
				queue[rear] = in;	// 入队
				rear = (rear + 1) % 100;
				map[in.x][in.y].used = 1;	// 标记使用了，防止重复访问
				// 记录路径，记录上一个点的坐标
				// in 来自 back 点
				p[in.x][in.y].x = back.x;
				p[in.x][in.y].y = back.y;
			}
		}
	}
	return flag;
}

// --------------------深搜算法要点------------------------
// 从起点开始入栈，入栈的要标记访问
// 按照顺时针方向向四方搜索
// 四个方向都走不通的时候，就出栈一个，继续探测
// 直到栈空，或者找到出口了
// DFS(map, m, n, &start, &end, &out))
int DFS(point map[MAXSIZE][MAXSIZE], 
		 int m, 
		 int n,
		 point *s, 
		 point *e,
		 stack *p)
{
	int flag = 0;
	int found, i;
	point back;
	point in;
	
	// 初始化访问标记
	clear(map, m, n);
	p->top = 0;
	p->base[p->top++] = *s;	// 起点入栈
	s->used = 1;			// 标记使用

	// 循环中的两个break，一个是找到出口了，一个是栈空了
	while (1)
	{
		// 拿栈顶元素来看看
		back = p->base[p->top - 1];

		// 先判断是不是出口，如果不是继续入栈
		if (back.x == e->x && back.y == e->y)
		{
			flag = 1;
			break;
		}
		// 不是出口，向四方搜索一个可通的路径
		for (i=0; i<4; i++)
		{
			found = 0;	// 标记没有找到通路
			in.x = back.x + dir[i].x;
			in.y = back.y + dir[i].y;
			// 如果可以通,
			if (find(map, in.x, in.y, m, n))
			{
				found = 1;
				p->base[p->top++] = in;	// 入栈
				map[in.x][in.y].used = 1;
				// 一定要标记使用了，防止重复搜索
				break;
			}
		}
		// 空栈就结束搜索
		// 如果搜索不到，就出栈一个
		if (!found) p->top--;
		if (p->top <= 0) break;
	}
	return flag;
}


// m * n 迷宫输入方法
// m n
// 接下去 m 行 n 列的 0 1
// 0 不通, 1 通
// 迷宫最大值 20 * 20

int main(void)
{
	point map[MAXSIZE][MAXSIZE];
	int n, m;
	point start;
	point end;
	point tmp;
	int x, y, i;
	
	printf("迷宫搜索");
	do {
		printf("\n请输入迷宫:");
		scanf("%d %d", &m, &n);
	} while (!check(m, n));

	input(map, m, n);
	system("cls");
	printf("迷宫如下：\n");
	showmap(map, m, n);
	printf("设置起点：");
	scanf("%d %d", &start.x, &start.y);
	printf("设置终点：");
	scanf("%d %d", &end.x, &end.y);
	// 不进行错误检查,请保证输正确
	
	// 广度搜索, path 记录路径
	point path[MAXSIZE][MAXSIZE];
	stack out;
	out.top = 0;

	// 开始广搜...
	if (BFS(map, m, n, &start, &end, path))
	{
		printf("\n广搜：找到出口！\n");
		init_quan(map, m, n);	// 初始化地图的权
		tmp = end;
		// 在 path 中提取路径, 从终点往起点搜，所以用栈保存
		while (tmp.x != INF || tmp.y != INF)
		{
			out.base[out.top++] = tmp;	// 入栈
			// 地图上也标记, USED 是必经之路
			map[tmp.x][tmp.y].quan = USED;
			// (x, y) 是前驱结点
			x = path[tmp.x][tmp.y].x;
			y = path[tmp.x][tmp.y].y;
			tmp.x = x;     tmp.y = y;
		}
		printf("入口：");
		while (out.top)
		{
			x = out.base[out.top - 1].x;
			y = out.base[out.top - 1].y;
			out.top--;
			printf(" -> (%d, %d)", x, y);
		}
		printf(" -> 出口\n路径如下图：\n");
		showmap(map, m, n);
	}
	else printf("广搜：找不到出口!\n");

	printf("\n按下回车键，开始深搜...");
	getchar();   getchar();

	// 开始深搜...
	if (DFS(map, m, n, &start, &end, &out))
	{
		printf("\n深搜：找到出口！\n入口：");
		init_quan(map, m, n);	// 初始化地图的权
		// 在栈中提取路径
		// 从栈底开始提取，说是栈，倒不如说是双端队列
		for (i=0; i<out.top; i++)
		{
			x = out.base[i].x;
			y = out.base[i].y;
			printf(" -> (%d, %d)", x, y);
			map[x][y].quan = USED;
		}
		printf(" -> 出口\n路径如下图\n");
		showmap(map, m, n);
	}
	else printf("深搜：找不到出口!\n");

	return 0;
}

/*
广搜：
首先起点入队，并标记使用
和起点有关的且没访问过的也入队，记得要标记使用
直到找到出口，或者队列空了，结束搜索

广搜记录路径：
开一个point类的二维数组，来记录前驱结点
就是记录当前点是从哪个点访问过来的
然后就可以从终点，找到起点，并输出

深搜：
起点入栈，并标记使用
找一个和栈顶结点有关的结点
并且这个结点没有使用过的，入栈，记得标记使用
如果找不到就从栈里弹出一个，因为这个点无路可走了
直到栈空，或者找到终点了, 结束搜索
栈里的元素就是路径了，

比较：广搜搜出来的路径是最短的，
*/
