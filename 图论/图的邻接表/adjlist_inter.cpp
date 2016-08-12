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
	if (graph->kind == DG || graph->kind == UDG)   // 无权图
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

void show(algraph *graph)		// 输出邻接表
{
	int i;
	vnode *cur;

	if (!graph) return ;
	for (i=0; i<graph->vexnum; i++)
	{
		printf("[%d][%c]", i, graph->vex[i]);
		cur = graph->vex[i].first;
		while (cur)
		{
			printf(" -> [%d]", cur->adj);
			if (graph->kind == DN || graph->kind == UDN)
				printf("[%d]", cur->wight);
			cur = cur->nextarc;
		}
		printf("\n");
	}
}

inline int find_vex(algraph *graph, elemtype *data)   // 查找结点地址
{
	int i;

	if (!graph) return NO;
	
	for (i=0; i<graph->vexnum; i++)
		if (graph->vex[i].data == *data) return i;
	return NO;
}

inline char *getname(algraph *graph, int addr)		// 查找结点名称
{
	if (!graph || addr < 0 || addr > graph->vexnum) 
		return NULL;
	return &(graph->vex[addr].data);
}

status insert_arc(algraph *graph, 
				  elemtype *st, 
				  elemtype *end, 
				  int wight)
{
	int flag;

	flag = insert_arc_s(graph, st, end, wight);

	// 如果是无向图, 还要加多一条边,
	if (graph->kind == UDG || graph->kind ==UDN)
		insert_arc_s(graph, end, st, wight);
	return flag;
}

// 插入一条边, 类似链表插入一个结点
status insert_arc_s(algraph *graph, 
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

status remove_arc(algraph *graph, 
				  elemtype *st, 
				  elemtype *end)
{
	int flag;

	flag = remove_arc_s(graph, st, end);
	// 如果是无向图, 还要多删一条边
	if (graph->kind == UDG || graph->kind == UDN)
		remove_arc_s(graph, end, st);
	return flag;
}

status remove_arc_s(algraph *graph, 
				  elemtype *st, 
				  elemtype *end)
{
	vnode *cur;
	vnode *pre;		// 前驱结点 [pre] -> [cur]
	int addr, addr2;

	if (!graph) return ERROR;
	addr = find_vex(graph, st);
	addr2 = find_vex(graph, end);
	if (addr == NO || addr2 == NO) return ERROR;

	pre = graph->vex[addr].first;
	cur = graph->vex[addr].first;
	while (cur)
	{
		// 类似遍历链表, 查找 addr2
		if (cur->adj == addr2) 
		{
			// 此时要释放 cur
			if (pre == cur)		// 第一个结点
			{
				graph->vex[addr].first = cur->nextarc;
				free(cur);
			}
			else	// 不是第一个结点
			{
				pre->nextarc = cur->nextarc;
				free(cur);
			}
			return OK;
		}
		pre = cur;
		cur = cur->nextarc;
	}
	return ERROR;
}

// 插入一个点
status insert_vex(algraph *graph, elemtype *name)
{
	if (graph->vexnum >= NUM) return ERROR;
	graph->vex[graph->vexnum].data = *name;
	graph->vex[graph->vexnum].first = NULL;
	graph->vexnum++;
	return OK;
}

// 删除一个点
status remove_vex(algraph *graph, elemtype *name)
{
	int i;
	vnode *cur;
	int addr = find_vex(graph, name);

	if (addr == NO) return ERROR;

	// 遍历邻接表, 把可到我的边, 全部删掉
	for (i=0; i<graph->vexnum; i++)
	{
		cur = graph->vex[i].first;
		while (cur)
		{
			if (cur->adj == addr)
			{
				cur = cur->nextarc;		// 先把 cur 走过去, 在删除
				remove_arc(graph, &(graph->vex[i].data), name);
			}
			else cur = cur->nextarc;
		}
	}

	// 把我可到达的边, 全部删掉
	cur = graph->vex[addr].first;
	while (cur)
	{
		graph->vex[addr].first = cur->nextarc;
		free(cur);
		cur = graph->vex[addr].first;
	}

	// 在头结构中删掉我, 类似顺序表的删除
	for (i=addr; i<graph->vexnum; i++)
	{
		graph->vex[i].data = graph->vex[i+1].data;
		graph->vex[i].first = graph->vex[i+1].first;
	}
	graph->vexnum--;

	return OK;
}

// 返回第一个邻接点
int first_vex(algraph *graph, elemtype *name)
{
	int addr;

	if (!graph) return NO;

	addr = find_vex(graph, name);	// 找到该点地址
	if (addr == NO) return NO;
	if (graph->vex[addr].first) 
		return graph->vex[addr].first->adj;
	else 
		return NO;
}

// 返回相对于 pos 的下一个邻接点
int next_vex(algraph *graph, elemtype *name, elemtype *pos)
{
	int addr, addr2;
	vnode *cur;

	if (!graph) return NO;

	addr = find_vex(graph, name);
	addr2 = find_vex(graph, pos);
	if (addr == NO || addr2 == NO) return NO;

	cur = graph->vex[addr].first;
	while (cur)
	{
		// [addr2] -> [?]
		if (cur->adj == addr2)
		{
			if (cur->nextarc)
				return cur->nextarc->adj;
			else return NO;
		}
		cur = cur->nextarc;
	}
	return NO;
}

char used[NUM];		// 访问标记数组

// 广度优先搜索
status BFS(algraph *graph, 
		   void (*visit)(algraph *graph, int n))
{
	int i;
	for (i=0; i<NUM; i++) used[i] = 0;   // 初始化数组

	for (i=0; i<graph->vexnum; i++)
		if (!used[i]) BFS_s(graph, i, visit);
	return OK;
}

// 打通单个分支
status BFS_s(algraph *graph, int i, 
			 void (*visit)(algraph *graph, int n))
{
	int queue[NUM];
	int rear = 0;
	int front = 0;
	int tmp;
	vnode *cur;

	// 根结点入队
	queue[rear] = i;
	rear = (rear + 1) % NUM;
	used[i] = 1;		// 标记访问了
	while (rear !=  front)		// 当队列不空
	{
		// 出队, 访问, 
		tmp = queue[front];
		front = (front + 1) % NUM;
		visit(graph, tmp);

		// 把与 tmp 有关的全部入队, 并标记
		cur = graph->vex[tmp].first;
		while (cur)
		{
			if (!used[cur->adj])
			{
				queue[rear] = cur->adj;
				rear = (rear + 1) % NUM;
				used[cur->adj] = 1;
			}
			cur = cur->nextarc;
		}
	}
	return OK;	
}


// 深度优先搜索
status DFS(algraph *graph, 
		   void (*visit)(algraph *graph, int n))
{
	int i;
	for (i=0; i<NUM; i++) used[i] = 0;   // 初始化数组
	
	for (i=0; i<graph->vexnum; i++)
		if (!used[i]) DFS_s(graph, i, visit);
	return OK;
}

// 深搜, 打通单个连通分支
status DFS_s(algraph *graph, int i, 
			 void (*visit)(algraph *graph, int n))
{
	vnode *cur;

	visit(graph, i);	// 访问后, 继续搜索
	used[i] = 1;
	cur = graph->vex[i].first;
	while (cur)			// 遍历链表, 递归深搜
	{
		if (!used[cur->adj])	// 如果没访问过
		{
			used[cur->adj] = 1;		// 标记使用
			DFS_s(graph, cur->adj, visit);	// 继续递归搜索
		}
		cur = cur->nextarc;
	}
	return OK;
}


// 深搜, 非递归
status DFS2(algraph *graph, 
			void (*visit)(algraph *graph, int n))
{
	int i;
	for (i=0; i<NUM; i++) used[i] = 0;
	
	for (i=0; i<graph->vexnum; i++)		// 非递归打通每个连通分支
		if (!used[i]) DFS2_s(graph, i, visit);
	return OK;
}

// 打通连通分支
// 对于每一个结点, 遍历其所有邻接点, 找到一个没访问过 k 的入栈
// 对于 k 也如此, 直到无路可走,
// 从栈里拿出一个, 继续上面的试探
status DFS2_s(algraph *graph, int f,
			void (*visit)(algraph *graph, int n))
{
	int stack[NUM + 10];
	int top = 0;
	char *name;

	stack[top++] = f;			// 根结点入栈
	while (top || f != NO)		// 当栈不空, 或有路走
	{
		while (f != NO)		// 朝一条路径走到底
		{
			if (!used[f])
			{
				visit(graph, f);	// 访问并标记
				used[f] = 1;
			}
			name = getname(graph, f);	// 获得当前结点名字
			// 向所有邻接点探测
			for (f = first_vex(graph, name);	// 从第一个邻接点,
				f != NO;
				f = next_vex(graph, name, getname(graph, f)))	
			{
				if (!used[f])	// 直到找到一个可走的, 就退出
				{
					stack[top++] = f;
					break;
				}
			}
		}
		// 此时 f == NO, 无路走了, 从栈里再拿出一个
		if (top) f = stack[--top];
	}
	return OK;
}
