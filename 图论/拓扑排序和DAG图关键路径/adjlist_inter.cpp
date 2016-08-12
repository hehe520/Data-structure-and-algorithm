#include <stdio.h>
#include <stdlib.h>
#include "adjlist.h"


// 创建图
status create(algraph *graph, 
			  int kind, 
			  elemtype *vexs, 
			  int n)
{
	int i;

	if (!graph || n >= NUM) return ERROR;

	graph->arcnum = 0;
	graph->kind = (gkind)kind;
	graph->vexnum = n;
	for (i=0; i<n; i++)
	{
		graph->vex[i].data = vexs[i];
		graph->vex[i].first = NULL;
	}
	return OK;
}

// 销毁图
status destory(algraph *graph)
{
	int i;
	vnode *del;

	if (!graph) return ERROR;

	// 遍历头结构
	for (i=0; i<graph->vexnum; i++)
	{
		// 遍历每个结点
		del = graph->vex[i].first;
		while (del)
		{
			graph->vex[i].first = del->nextarc;
			free(del);
			del = graph->vex[i].first;
		}
	}
	graph->arcnum = 0;
	graph->vexnum = 0;
	return OK;
}

void input(algraph *graph)		// 输入
{
	char st, end;
	int wight;

	if (!graph) return ;
	if (graph->kind == DG)   // 无权图
	{
		while (1)
		{
			fflush(stdin);
			scanf("%c %c", &st, &end);
			if (st == '0' && end == '0') break;
			insert_arc(graph, &st, &end, 0);
		}
	}
	else
	{
		while (1)
		{
			fflush(stdin);
			scanf("%c %c %d", &st, &end, &wight);
			if (st == '0' && end == '0' && !wight) break;
			insert_arc(graph, &st, &end, wight);
		}
	}
	fflush(stdin);
}

inline int find_vex(algraph *graph, elemtype *data)   // 查找结点地址
{
	int i;

	if (!graph) return NO;
	
	for (i=0; i<graph->vexnum; i++)
		if (graph->vex[i].data == *data) return i;
	return NO;
}

// 插入一条边, 类似链表插入一个结点
status insert_arc(algraph *graph, 
				  elemtype *st, 
				  elemtype *end, 
				  int wight)
{
	int addr, addr2;
	vnode *cur;

	if (!graph) return ERROR;

	addr = find_vex(graph, st);			// 找到起点地址
	addr2 = find_vex(graph, end);		// 找到终点地址
	if (addr == NO || addr2 == NO) return ERROR;

	cur = graph->vex[addr].first;
	while (cur)		// 如果已有该边, 则不重复插入,
	{
		if (cur->adj == addr2) return ERROR;
		cur = cur->nextarc;
	}

	// 为了方便, 采用头插法
	cur = (vnode *)malloc(sizeof(vnode));
	cur->adj = addr2;
	cur->wight = wight;
	cur->info = NULL;
	cur->nextarc = graph->vex[addr].first;
	graph->vex[addr].first = cur;
	graph->arcnum++;

	return OK;
}
