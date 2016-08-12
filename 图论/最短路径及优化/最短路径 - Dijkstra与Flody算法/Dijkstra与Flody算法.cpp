/* -------------------------
// 最短路径算法
// 广搜
// 迪杰斯特拉 (dijkstra)
// 佛洛依德 (floyd)
// 附加深搜非递归的简洁代码
----------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mgraph.h"		// 图的邻接矩阵头文件

#define MAX MAX_NUM

// 广度优先搜索, 搜索边最少的路径, 用 pre 记录路径
status BFS(mgraph *graph, char start, char end, int pre[])
{
	int queue[50];		// 简单队列
	int rear = 0;
	int front = 0;
	char used[MAX];			// 访问标记数组
	int addr1, addr2;		// 起点, 终点
	int i, tmp;
	int cur = 0;

	for (i=0; i<graph->vexnum; i++) used[i] = 0;
	addr1 = find_vex(graph, &start);	// 查找地址
	addr2 = find_vex(graph, &end);
	if (addr1 == NO || addr2 == NO) return ERROR;
	if (addr1 == addr2) return ERROR;	// 自己到自己也不行

	queue[rear] = addr1;		// 根结点入队
	rear = (rear + 1) % 50;
	used[addr1] = 1;
	pre[addr1] = NO;		// 起点没有前驱
	while (rear != front)
	{
		tmp = queue[front];		// 遍历 tmp 的邻接点, 寻找目标
		front = (front + 1) % 50;
		for (i=0; i<graph->vexnum; i++)
		{
			// 没有访问过的邻接点, 全部入队
			// 以 tmp 为中心, i 为邻接点
			if (!used[i] && graph->arcs[tmp][i].adj != INF)
			{
				pre[i] = tmp;		// 记录路径 - 前驱结点
				if (addr2 == i) return OK;	// 找到终点就退出
				used[i] = 1;
				queue[rear] = i;
				rear = (rear + 1) % 50;
			}
		}
	}
	return ERROR;
}

// 根据 pre[] 记录的前驱结点, 从终点往回走, 获得路径
void extract(mgraph *graph, char *path, int *pre, int end)
{
	int n;
	char buf[MAX + 5];

	n = MAX_NUM + 5;
	buf[--n] = '\0';
	while (end != NO)		// 从终点往回走
	{
		buf[--n] = graph->vexs[end];
		end = pre[end];
	}
	strcpy(path, buf + n);
}

void output(char *str)		// 输出路径
{
	while (*str != '\0')
	{
		printf("%c", *str);
		str++;
		if (*str != '\0') printf(" -> ");
	}
	printf("\n");
}

void DFS(mgraph *graph)		// 非递归深搜
{
	int stack[MAX + 10];
	int top = 0;
	int used[MAX];
	int i;
	int s = 0;		// 搜索起点设为 0

	for (i=0; i<graph->vexnum; i++) used[i] = 0;

	while (top || s != NO)
	{
		while (s != NO)		// 朝一条路, 搜到尽头
		{
			if (!used[s])	//  没访问过的才可以入栈
			{
				stack[top++] = s;
				used[s] = 1; 
				printf("%c, ", graph->vexs[s]);
			}
			// 以 s 为中心, 遍历 s 的邻接点 i
			for (i=0; i<graph->vexnum; i++)
			{
				if (!used[i] && graph->arcs[s][i].adj != INF)
				{
					s = i;   break;   // 焦点转移到 i    
				}
			}
			if (i == graph->vexnum) s = NO;
		}
		if (top) s = stack[--top];
	}
}


// -----------------dijkstra 最短路径算法-------------------
// 
// 算法思想: 贪心,  用 f(A, B) 表示 A 到 B 的距离
// 例如, f(A, B) = 10,   f(A, C) = 1,  f(C, B) = 1
// 若发现, f(A, C) + f(C, B) < f(A, B), 则更新 f(A, B)
// 上面的 f(C, B) 就是待寻找的对象
// 每次去找目前最小, 且没有访问过的结点当作 f(C, B) 去松弛邻接点
// 所以用 dis[] 记录目前的最短路径长度,  用used[] 记录访问标记
// pre[] 记录前驱结点, 记录路径
//
status dij_shortpath(mgraph *graph, char st, int dis[], int pre[])
{
	char used[MAX];		// 访问标记
	int add;
	int min, pos;
	int i, j;

	add = find_vex(graph, &st);
	if (add == NO) return ERROR;
	
	for (i=0; i<graph->vexnum; i++)		// 初始化距离
	{ dis[i] = INF;   used[i] = 0; }

	dis[add] = 0;		// 设置起点
	pre[add] = NO;		// 起点无前驱
	for (i=0; i<graph->vexnum; i++)		// n 个点, 松弛 n 次
	{
		// 找到一个目前最小距离, 且没有访问过的结点
		// 每次都拿最小的距离去松弛, 最后结果当然最小
		min = INF;
		for (j=0; j<graph->vexnum; j++)
		{
			if (!used[j] && dis[j] < min)
			{ pos = j;   min = dis[j]; }
			// min 记录目前到 pos 的距离
		}
		if (min == INF) break;		// 此时找不到最小的了
		used[pos] = 1;	// 标记 pos 使用了

		// 以 pos 为中心, 松弛未访问的邻接点 j
		for (j=0; j<graph->vexnum; j++)
		{
			// 对于访问过的点, 没必要再松弛, 因为 a + min(正数)不可能小于 a
			if (!used[j] && min + graph->arcs[pos][j].adj < dis[j])
			{
				dis[j] = min + graph->arcs[pos][j].adj;
				pre[j] = pos;		// 同时记录谁松弛到我
			}
		}
	}
	if (min == INF) return ERROR;
	else return OK;
}

// -------------------floyd 最短路-----------------------
// 先拿一个点, 再去进行 n 次试探, 不能先拿 n 个点松弛,
// dis[a][b] 矩阵记录任意两点的距离
// pre[a][b] 表示 a 到 b 的路径上, b 前驱结点
status floyd_shortpath(mgraph *g, int (*dis)[MAX], int (*pre)[MAX])
{
	int i, j, k;
	int num = g->vexnum;;

	for (i=0; i<num; i++)	// 初始化第一个矩阵
	{
		for (j=0; j<num; j++)
		{
			dis[i][j] = g->arcs[i][j].adj;
			if (dis[i][j] != INF) pre[i][j] = i;
			else pre[i][j] = NO;
		}
	}
	for (k=0; k<num; k++)		// 先拿一个点
		for (i=0; i<num; i++)	// 在矩阵中进行松弛
			for (j=0; j<num; j++)
				if (i != j && dis[i][k] + dis[k][j] < dis[i][j])
				{
					// i 和 j 不能相等, 因为自己到自己的路径没意义
					// 在 i 到 j 间添加 k 进行尝试
					// 如发现 i..k + k..j < i..j, 则进行更新
					dis[i][j] = dis[i][k] + dis[k][j];
					pre[i][j] = pre[k][j];
					// 在 i 到 j 的路径上发现, 有 k 到 j 更小
					// 则更新路径 i..j 为 i..k..j
				}
	return OK;
}

int main(void)
{
	mgraph graph;
	int dis[MAX];			// dijkstra 最短距离
	int pre[MAX];			// dijkstra 记录路径
	char path[MAX][MAX];	// dijkstra 路径的字符串
	// path[n] 是定点到 n 的字符串地址

	int dis2[MAX][MAX];			// floyd 最短距离
	int pre2[MAX][MAX];			// floyd 记录路径
	char path2[MAX][MAX][MAX];	// floyd 路径字符串
	int kind;
	int n, i, j;
	nametype vex[MAX];	// 点集
	char start, end;
	int add;

	printf("请输入图的类型：(0.有向网, 1.无向网):");
	scanf("%d", &kind); 
	printf("请输入顶点个数及名称:");
	scanf("%d", &n);   fflush(stdin);
	for (i=0; i<n; i++) scanf("%1c", vex + i);
	create(&graph, kind, n, vex);	// 创建图
	printf("请输入边集：\n");
	input(&graph);					// 批量输入
	system("cls");
	printf("\n请输入起点和终点: ");
	scanf("%c %c", &start, &end);

	add = find_vex(&graph, &end);			// 终点地址 add
	printf("\n边数最少的路径: ");			// 广搜最短路
	if (BFS(&graph, start, end, pre))
	{
		// pre 记录着路径, 提取到 path[0] 中
		extract(&graph, path[0], pre, add);
		output(path[0]);		// 输出路径
	}
	else printf("未找到!\n");
	printf("深度优先遍历: ");   DFS(&graph);
	
	printf("\n\ndijkstra 最短路: ");
	if (dij_shortpath(&graph, start, dis, pre))
	{	
		// dis 记录着 A 到任何点的最短距离
		// pre 记录着 A 到任何点的路径, 可进行多次查询
		for (i=0; i<graph.vexnum; i++)
			extract(&graph, path[i], pre, i);
		output(path[add]);
		printf("最短路径长度: %d\n", dis[add]);
	}
	else printf("未找到!\n");

	if (floyd_shortpath(&graph, dis2, pre2))
	{
		// dis2 记录这任意两点的路径长度
		// 进行一次 flody 算法, 就可进行多次查询,  
		n = find_vex(&graph, &start);
		if (dis2[n][add] != INF) 
		{
			printf("\nfloyd 最短路: ");
			for (i=0; i<graph.vexnum; i++)	// 枚举起点
			{
				for (j=0; j<graph.vexnum; j++)	// 枚举终点
				{
					extract(&graph, path2[i][j], pre2[i], j);
				}
			}
			output(path2[n][add]);
			printf("最短路径长度: %d\n", dis2[n][add]);
		}
		else printf("\nfloyd 最短路: 未找到!\n");
	}
	return 0;
}
