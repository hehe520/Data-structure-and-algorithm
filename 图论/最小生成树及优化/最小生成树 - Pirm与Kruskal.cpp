/* -----------------------------------
// 最小生成树的两个算法
// 采用邻接矩阵储存无向带权图
// 
// 主要函数:
// prim();       普里姆 (prim)
// kruskal();    克鲁斯卡尔 (kruskal)
-------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int status;
typedef char elemtype;
typedef char ntype;
#define ERROR 0
#define OK 1
#define NUM 30			// 最大结点数
#define INF 0xFFFFFFF	// 表示无穷大
#define NO -1			// 没找到

// kruakal 的辅助空间
// 其实 kruskal 算法用邻接表更好
typedef struct		// 记录全部的边
{
	int from;		// 起点
	int to;			// 终点
	int wight;		// 权值
}recond2;


// 邻接矩阵的一个结点
typedef struct 
{
	int wight;		// 权值
	char *info;		// 附加信息
}arc;

// ----------邻接矩阵-----------
typedef struct
{
	arc arcs[NUM][NUM];		// 边集
	ntype name[NUM];		// 点集
	int vexnum;			// 顶点数
	int arcnum;			// 边数
}mgraph;

void init(mgraph *graph)		// 初始化图
{
	int i, j;

	for (i=0; i<NUM; i++)
	{
		for (j=0; j<NUM; j++)
		{
			// 权值无穷大, 表示不通
			graph->arcs[i][j].wight = INF;
			graph->arcs[i][j].info = NULL;
		}
		graph->name[i] = '\0';
		graph->arcnum = 0;
		graph->vexnum = 0;
	}
}

// 查找 data 的下标是多少
short int find_addr(mgraph *graph, elemtype *data)
{
	short int i;
	for (i=0; i<graph->vexnum; i++)
		if (graph->name[i] == *data) return i;
	return NO;
}

// -----------kruskai 的并查集-------------
int parent[NUM];		// 指示双亲结点

void init_set(int n)
{
	int i;
	for (i=0; i<n; i++) parent[i] = i;
}

int root(int a)		// 递归压缩效率较低
{
	if (parent[a] != a)
		parent[a] = root(parent[a]);
	return parent[a];
}

int root2(int a)
{
	int i = a, tmp;
	while (parent[a] != a) a = parent[a];	// 查找
	while (i != a)		// 非递归压缩路径
	{
		tmp = parent[i];
		parent[i] = a;
		i = tmp;
	}
	return a;
}

void union_set(int a, int b)
{
	a = root2(a);	// 查找老大
	b = root2(b);
	if (a != b) parent[a] = b;
	// 老大不一样就合并
	// parent[b] = a;  也可
	// 因为路径压缩的时候, 会降低深度
}

void sort(recond2 *re, int n)		// 冒泡排序
{
	int i, j;
	recond2 tmp;
	char flag;

	for (i=0; i<n-1; i++)
	{
		flag = 1;
		for (j=0; j<n-2; j++)
		{
			if (re[j].wight > re[j+1].wight)
			{
				tmp = re[j];
				re[j] = re[j+1];
				re[j+1] = tmp;
				flag = 0;
			}
		}
		if (flag) break;
	}
}

void input(mgraph *graph)		// 输入模块
{
	char vex[NUM + 2];
	int len, i;
	int wight;
	char a, b;
	short int addr1, addr2;

	printf("请输入顶点集: ");
	scanf("%s", vex);   fflush(stdin);
	len = strlen(vex);
	graph->vexnum = len;
	for (i=0; i<len; i++)
		graph->name[i] = vex[i];
	printf("请输入边集: ");
	while (1)
	{
		fflush(stdin);
		scanf("%c %c %d", &a, &b, &wight);
		if (a == '0' && b == '0' && wight == 0) break;
		addr1 = find_addr(graph, &a);
		addr2 = find_addr(graph, &b);
		if (!(addr1 == NO || addr2 == NO) && wight > 0)
		{
			graph->arcs[addr1][addr2].wight = wight;
			graph->arcs[addr2][addr1].wight = wight;
			graph->arcnum++;
		}
	}
	fflush(stdin);
}

// 返回第一个邻接点
inline int first_vex(mgraph *graph, int addr)
{
	int i;
	if (addr < 0 || addr >= graph->vexnum) return NO;

	for (i=0; i<graph->vexnum; i++)
		if (graph->arcs[addr][i].wight != INF)
			return i;
	return NO;
}

// 返回下一个邻接点
inline int next_vex(mgraph *graph, int addr, int pos)
{
	int i;
	if (addr < 0 || addr >= graph->vexnum) return NO;
	if (pos < 0 || pos >= graph->vexnum) return NO;

	for (i=pos+1; i<graph->vexnum; i++)
		if (graph->arcs[addr][i].wight != INF) 
			return i;
	return NO;
}

void output(mgraph *graph, int a, int b)	// 输出一条边
{
	printf("(%c, %c), ", graph->name[a], graph->name[b]);
	// 这里直接输出到屏幕
	// 也可以用一个数组保存边集
	// 或者用邻接矩阵, 邻接表保存边集
	// 或者用树的存储方法保存
	// 或者...
}


// ---------------最小生成树 - 普里姆算法--------------------
// 
// 算法:  每次从 0 集中选一个权值最小的结点 cur
// 将 cur 并入 1 集, 从 cur 出发遍历邻接点, 做松弛操作
// 直到 n 个结点都属于 1 集, 共执行 n 次操作
// 普里姆算法与边数无关, 适合稠密图, 
status prim(mgraph *graph)
{
	typedef struct		// 辅助空间开三个数组也可
	{
		int pre;		// 记录前驱结点, 也就是谁更新到我的结点
		int wight;		// 记录最小权值
		char set;		// 记录所在集合
	}recond;
	recond *re;
	int i, j;
	int start;
	int min, cur;
	int result = 0;		// 返回权值
	
	// 辅助空间, 并初始化
	re = (recond *)malloc(graph->vexnum * sizeof(recond));
	if (!re) return ERROR;
	for (i=0; i<graph->vexnum; i++)
	{
		re[i].pre = i;			// 开始没有前驱结点
		re[i].set = 0;			// 开始属于 0 集合
		re[i].wight = INF;		// 各个顶点不通
	}
	start = 0;			// 随机从一个点出发, start = 1,2,3...都可
	re[start].wight = 0;		// 自己到自己的距离是 0

	for (i=0; i<graph->vexnum; i++)		// 选择 n 个点
	{
		// 从 0 集合中, 选择一个权值最小的
		min = INF;   cur = NO;
		for (j=0; j<graph->vexnum; j++)
		{
			if (re[j].set == 0 && re[j].wight < min)
			{
				min = re[j].wight;	// 找到最小的
				cur = j;			// 记录位置
			}
		}
		if (cur == NO) return result;	 // 错误: 不是一个连通图
		else if (start != cur)			 // 输出一条边, 累加权值
		{
			// re[cur].pre 是 cur 的前驱结点, 类似树的 parent
			output(graph, re[cur].pre, cur);
			result += graph->arcs[ re[cur].pre ][cur].wight;
		}
		re[cur].set = 1;	// 使用过的点, 并入 1 集合

		// 遍历 cur 的邻接点, 更新权值, 类似最短路的松弛操作,
		// 当前结点是 cur, cur 到 j 进行试探, 
		for (j = first_vex(graph, cur);
			j != NO;
			j = next_vex(graph, cur, j))
		{
			// cur 到 j 的权值比原来小, 就更新
			if (graph->arcs[cur][j].wight < re[j].wight)
			{
				re[j].pre = cur;		// 更新前驱结点
				re[j].wight = graph->arcs[cur][j].wight;
			}
		}
	}
	free(re);
	return result;
}


// ---------------最小生成树 - 克鲁斯卡尔算法----------------
// 
// 每次去拿权值最小的且不形成回路的边, 连通两个分支
// 直到拿了 n - 1 条
// 并查集判断回路,  [from][to][wght] 保存边集
// 克鲁斯卡尔算法与边数有关, 适合稀疏图
int kruskal(mgraph *graph)
{
	recond2 *re;
	int i, j;
	int cur = 0;
	int result = 0;
	int count = 0;
	int a, b;

	re = (recond2 *)malloc(graph->vexnum * sizeof(recond2));
	if (!re) return ERROR;

	// 从邻接矩阵中提取所有边到 re 中
	// 无向图矩阵对称, 只提取下三角
	for (i=0; i<graph->vexnum; i++)
		for (j=0; j<i; j++)
			if (graph->arcs[i][j].wight != INF)
			{
				re[cur].wight = graph->arcs[i][j].wight;
				re[cur].from = i;
				re[cur++].to = j;
			}
	sort(re, cur);		// 按权值进行冒泡排序

	init_set(graph->vexnum);	// 要记得初始化并查集
	for (i=0; i<cur; i++)
	{
		a = re[i].from;   b = re[i].to;		// 获得两个端点
		if (root2(a) != root2(b))
		{
			union_set(a, b);		// 合并
			output(graph, a, b);	// 输出边
			result += re[i].wight;
			count++;
			// n 个点至少要 n-1 条边即可连通
			if (count == graph->vexnum - 1) break;
		}
	}
	return result;
}

int main(void)
{
	mgraph graph;	// 图的邻接矩阵
	int wight;

	init(&graph);
	printf("请输入一个无向带权图: \n");
	input(&graph);

	printf("\nprim 算法:\n最小生成树边集: ");
	wight = prim(&graph);					// 普里姆算法
	printf("\n总权值: %d\n", wight);

	printf("\n\nkruskal 算法:\n最小生成树边集: ");
	wight = kruskal(&graph);				// 克鲁斯卡尔算法
	printf("\n总权值: %d\n\n", wight);

	return 0;
}
