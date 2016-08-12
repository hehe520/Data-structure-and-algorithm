#include <stdio.h>
#include "mgraph.h"

// 创建网
status create(mgraph *graph, int kind, int n, nametype *vex)
{
	int i, j;

	// 合法检查
	if (!graph) return ERROR;
	if (n >= MAX_NUM || n <= 0) return ERROR;

	// 初始化成员
	graph->arcnum = 0;
	graph->kind = (graphkind)kind;
	graph->vexnum = n;
	for (i=0; i<n; i++) graph->vexs[i] = vex[i];

	// 网是带权图, 不通是无穷大 INF
	for (i=0; i<n; i++)
		for (j=0; j<n; j++)
		{
			// 一开始是不通的
			graph->arcs[i][j].adj = INF;
			graph->arcs[i][j].info = NULL;
		}
	return OK;
}

// 创建边集
status input(mgraph *graph)
{
	nametype a, b;
	int quan;	// 权值

	if (!graph) return ERROR;
	while (1)   // 构造弧集
	{
		fflush(stdin);
		scanf("%c %c %d", &a, &b, &quan);
		if (a == '0' && b == '0') break;
		insertarc(graph, &a, &b, quan);
		if (graph->kind == UDN) insertarc(graph, &b, &a, quan);
	}
	fflush(stdin);
	return OK;
}

// 寻找 data 的位序
int find_vex(mgraph *graph, nametype *data)
{
	int i;

	for (i=0; i<graph->vexnum; i++)
	{
		if (graph->vexs[i] == *data)
			return i;
	}
	return NO;
}

// 插入边
status insertarc(mgraph *graph, nametype *a, nametype *b, int quan)
{
	int x, y;

	if (!graph) return ERROR;
	x = find_vex(graph, a);
	y = find_vex(graph, b);
	if (x == NO || y == NO) return ERROR;

	graph->arcs[x][y].adj = quan;
	if (graph->kind == UDN) graph->arcs[y][x].adj = quan;
	return OK;
}
