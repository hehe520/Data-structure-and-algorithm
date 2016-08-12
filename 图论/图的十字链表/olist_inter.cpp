#include <stdio.h>
#include <stdlib.h>
#include "olist.h"

status create(olgraph *graph, elemtype *vexs, int n)
{
	int i;

	if (!graph) return ERROR;
	graph->arcnum = 0;
	graph->vexnum = n;
	for (i=0; i<n; i++)
	{
		graph->vex[i].data = vexs[i];
		graph->vex[i].fromarc = NULL;
		graph->vex[i].toarc = NULL;
	}
	return OK;
}

status destory(olgraph *graph)
{
	vnode *cur;
	int i;

	if (!graph) return ERROR;
	for (i=0; i<graph->vexnum; i++)
	{
		// 按照第一条出边来释放
		cur = graph->vex[i].fromarc;
		while (cur)
		{
			graph->vex[i].fromarc = cur->fromarc;
			free(cur);
			cur = graph->vex[i].fromarc;
		}
	}
	return OK;
}

void input(olgraph *graph)
{
	char st, end;

	if (!graph) return ;
	while (1)
	{
		fflush(stdin);
		scanf("%c %c", &st, &end);
		if (st == '0' && end == '0') break;
		insert_arc(graph, &st, &end);
	}
	fflush(stdin);
}

void show(olgraph *graph)
{
	int i;
	vnode *cur;

	if (!graph) return ;
	for (i=0; i<graph->vexnum; i++)
	{
		printf("[%d][%c]", i, graph->vex[i]);
		cur = graph->vex[i].fromarc;
		while (cur)
		{
			printf(" -> [%d][%d]", cur->from, cur->to);
			cur = cur->fromarc;
		}
		printf("\n");
	}
}

int find_vex(olgraph *graph, elemtype *data)
{
	int i;
	for (i=0; i<graph->vexnum; i++)
		if (graph->vex[i].data == *data) return i;
	return NO;
}

// 插入一条边
status insert_arc(olgraph *graph, elemtype *st, elemtype *end)
{
	vnode *n;
	vnode *cur;
	int from, to;

	from = find_vex(graph, st);
	to = find_vex(graph, end);
	if (from == NO || to == NO) return ERROR;

	n = (vnode *)malloc(sizeof(vnode));
	n->from = from;   n->to = to;
	n->fromarc = n->toarc = NULL;
	n->info = NULL;
	
	// 连到十字链表中, 先连接横向
	cur = graph->vex[from].fromarc;
	while (cur)
	{
		if (cur->fromarc == NULL)
		{
			cur->fromarc = n;
			break;
		}
		cur = cur->fromarc;
	}
	if (!cur) graph->vex[from].fromarc = n;

	// 再连接纵向
	cur = graph->vex[to].toarc;
	while (cur)
	{
		if (cur->to == NULL)
		{
			cur->toarc = n;
			break;
		}
		cur = cur->toarc;
	}
	if (!cur) graph->vex[to].toarc = n;
	return OK;
}

status remove_arc(olgraph *graph, elemtype *st, elemtype *end)
{
	int from, to;
	vnode *cur, *cur2 ,*pre;

	from = find_vex(graph, st);
	to = find_vex(graph, end);
	if (from == NO || to == NO) return ERROR;

	cur = graph->vex[from].fromarc;
	pre = cur;
	while (cur)
	{
		if (cur->to == to)
		{
			if (pre == cur)
				graph->vex[from].fromarc = cur->fromarc;
			else pre->fromarc = cur->fromarc;
			// 删除纵向连接
			cur2 = graph->vex[to].toarc;
			pre = cur2;
			while (cur2)
			{
				if (cur2->from == from)
				{
					if (cur2 == pre)
						graph->vex[to].toarc = cur2->toarc;
					else pre->toarc = cur2->toarc;
				}
				cur2 = cur->toarc;
			}
			free(cur);
			return OK;
		}
		pre = cur;
		cur = cur->fromarc;
	}
	return OK;
}

int first_vex(olgraph *graph, elemtype *name)
{
	int addr;

	addr = find_vex(graph, name);
	if (addr == NO) return NO;
	if (graph->vex[addr].fromarc)
		return graph->vex[addr].fromarc->to;
	else return NO;
}

int next_vex(olgraph *graph, elemtype *name, elemtype *pos)
{
	int addr, addr2;
	vnode *cur;

	addr = find_vex(graph, name);
	addr2 = find_vex(graph, pos);
	if (addr == NO || addr2 == NO) return NO;
	cur = graph->vex[addr].fromarc;
	while (cur)
	{
		if (cur->to == addr2)
		{
			if (cur && cur->fromarc)
				return cur->fromarc->to;
			else return NO;
		}
	}
	return NO;
}
