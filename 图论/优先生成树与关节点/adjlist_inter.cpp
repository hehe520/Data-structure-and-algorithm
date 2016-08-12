#include <stdio.h>
#include <stdlib.h>
#include "adjlist.h"

int used[NUM];		// 访问标记数组, 移到函数体内也可
int low[NUM];		// 记录可达最小编号
int count;			// 统计结点个数

// 输出关节点到 back
// 查找关节点算法, 
// 1, 根有不止一棵子树, 根是关节点
// 2. 非根结点, 存在一棵子树没有回边, 是关节点
// 对 1. 的判断, 从第一个子树出发, 没有全部访问掉, 说明存在多个子树
// 对 2. 的判断, 如何发现回边?
// 设 used[] 数组, 按照深度遍历顺序由小到大编号,
// 有了这个升序的性质, 那么对于任何子树的编号, 不可能小于根
// 若要是小于根了, 说明有回边,
// 设 low[] 数组. 记录可达的结点的最小编号,
// 显然, 对于一个子根 pos, 若存在一个子树 v, 使得 low[v] >= used[pos]
// 则 pos 是关节点
status DFS_node(algraph *graph, char *back)
{
	int i;
	vnode *cur;

	for (i=0; i<graph->vexnum; i++) used[i] = NO;
	used[0] = count = 0;	// 拿 0 号结点作为生成树的根

	cur = graph->vex[0].first;
	back = DFS_node_s(graph, cur->adj, back);	// 打通单个子树
	if (count < graph->vexnum - 1)		// 说明没有访问完全
	{
		*back++ = graph->vex[0].data;		// 此时根时关节点
		cur = cur->nextarc;
		while (cur)
		{
			if (!used[cur->adj])		// 访问没访问的结点
				DFS_node_s(graph, cur->adj, back);
			cur = cur->nextarc;
		}
	}
	*back = '\0';
	return OK;
}

char *DFS_node_s(algraph *graph, int pos, char *back)
{
	vnode *cur;
	int min;	// 当前结点的可达最小结点

	// 给第 pos 的结点编号, 最小只能回到自己
	min = used[pos] = ++count;
	cur = graph->vex[pos].first;
	while (cur)
	{
		if (used[cur->adj] == NO)
		{
			back = DFS_node_s(graph, cur->adj, back);
			// 先要求出 low[cur->adj], 下面回溯部分, 类似二叉树的后序遍历

			// 遍历 cur 的邻接点, 求一个最小可达结点 min
			if (low[cur->adj] < min) min = low[cur->adj];

			// pos 相当于子树根, cur 都是邻接点
			// 按照定义, 存在一个子树, pos 即是关节点
			if (low[cur->adj] >= used[pos]) 
				*back++ = graph->vex[pos].data;
		}
		else if (used[cur->adj] < min) min = used[cur->adj];
		// 对于访问过的边, 同样进行回边探测
		cur = cur->nextarc;
	}
	low[pos] = min;		// 回溯的时候, 修改low[pos]
	return back;
}


status BFS(algraph *graph, tree **t)	// 广度优先生成树
{
	int i;
	cnode *n;
	cnode *p = NULL;	// 不能修改 t, 用 p 代表 t

	*t = NULL;
	for (i=0; i<NUM; i++) used[i] = 0;

	for (i=0; i<graph->vexnum; i++)
	{
		if (!used[i])
		{
			used[i] = 1;
			// 先建好根结点, 在根结点上建立孩子结点
			n = (cnode *)malloc(sizeof(cnode));
			if (!n) return ERROR;
			n->brother = n->child = NULL;
			n->data = graph->vex[i].data;
			if (p)
			{
				p->brother = n;		// 已经有根了, 在兄弟上建立
				p = p->brother;
			}
			else p = *t = n;		// 第一次建立根
			BFS_s(graph, i, &p);	// 打通一个连通分支
		}
	}
	return OK;
}

status BFS_s(algraph *graph, int i, tree **t)
{
	vnode *cur;		// 图的当前结点
	cnode *now;		// 树的当前结点
	cnode *pre;		// now 的前驱结点
	int queue[60];
	int rear = 0;
	int front = 0;
	int first = 1;
	int tmp;

	queue[rear] = i;	// 根结点入队
	rear = (rear + 1) % 60;
	while (rear != front)
	{
		tmp = queue[front];
		front = (front + 1) % 60;
		cur = graph->vex[tmp].first;
		while (cur)		// 遍历所有邻接点
		{
			if (!used[cur->adj])
			{
				used[cur->adj] = 1;
				// 建立新空间
				now = (cnode *)malloc(sizeof(cnode));
				now->brother = now->child = NULL;
				now->data = graph->vex[cur->adj].data;
				if (first)		// 如果是首次, 在孩子上建立
				{
					first = 0;				//    t
					(*t)->child = now;		//   /
					pre = (*t)->child;		// child (pre) -> ...
				}
				else	// 不是首次在兄弟上建立
				{
					pre->brother = now;
					pre = now;
				}
				queue[rear] = cur->adj;		// 队列记录
				rear = (rear + 1) % 60;
			}
			cur = cur->nextarc;
		}
	}
	return OK;
}

status DFS(algraph *graph, tree **t)	// 深度优先生成树
{
	int i;
	cnode *n;
	cnode *p = NULL;

	for (i=0; i<NUM; i++) used[i] = 0;
	for (i=0; i<graph->vexnum; i++)
	{
		if (!used[i])
		{
			used[i] = 1;
			// 建立根结点
			n = (cnode *)malloc(sizeof(cnode));
			n->brother = n->child = NULL;
			n->data = graph->vex[i].data;

			if (p)		// 不是第一次建根, 那就是兄弟结点
			{
				p->brother = n;
				p = n;
			}
			else p = *t = n;
			DFS_s(graph, i, &p);
		}
	}
	return OK;
}

status DFS_s(algraph *graph, int i, tree **t)
{
	vnode *cur;
	cnode *n;
	static cnode *now;	// 不能在递归中改变
	static char f = 1;	// 只执行一次

	cur = graph->vex[i].first;
	while (cur)		// 遍历状态树
	{
		if (!used[cur->adj])	// 剪枝
		{
			used[cur->adj] = 1;
			// 建立孩子结点
			n = (cnode *)malloc(sizeof(cnode));
			n->child = n->brother = NULL;
			n->data = graph->vex[cur->adj].data;
			if (f)
			{
				f = 0;
				(*t)->child = n;
				now = n;	// now == 首孩子
			}
			else
			{
				now->brother = n;
				now = n;
			}
			DFS_s(graph, cur->adj, &now);
		}
		cur = cur->nextarc;
	}
	return OK;
}

status DFS2(algraph *graph, tree **t)	// 深度优先生成树(非递归)
{
	int i;
	cnode *n;
	cnode *p = NULL;

	for (i=0; i<NUM; i++) used[i] = 0;
	for (i=0; i<graph->vexnum; i++)
	{
		if (!used[i])
		{
			used[i] = 1;
			n = (cnode *)malloc(sizeof(cnode ));
			n->child = n->brother = NULL;
			n->data = graph->vex[i].data;
			if (p)
			{
				p->brother = n;
				p = n;
			}
			else p = *t = n;
			DFS2_s(graph, i, &p);
		}
	}
	return OK;
}

status DFS2_s(algraph *graph, int i, tree **t)
{
	vnode *cur;
	int stack[60];
	int top = 0;
	char f = 1;
	int tmp;
	cnode *n;
	cnode *now;

	stack[top++] = i;	// 根结点入栈
	cur = graph->vex[i].first;
	while (top || cur)
	{
		// 遍历邻接点, 找到一个可以走的路
		while (cur)
		{
			if (!used[cur->adj])	// 找到了
			{
				used[cur->adj] = 1;
				n = (cnode *)malloc(sizeof(cnode));
				n->brother = n->child = NULL;
				n->data = graph->vex[cur->adj].data;
				if (f)		// 第一次就连载首孩子
				{
					f = 0;
					(*t)->child = n;
					now = n;
				}
				else	// 不是第一次就连在兄弟上
				{
					now->brother = n;
					now = n;
				}
				// 入栈记录, 并去到邻接点
				// 把 cur->adj 作为起点, 继续搜索
				stack[top++] = cur->adj;
				cur = graph->vex[cur->adj].first;
			}
			else cur = cur->nextarc;
		}
		if (top) 
		{
			tmp = stack[--top];		// 往回退一格, 继续搜索
			cur = graph->vex[tmp].first;
		}
	}

	return OK;
}


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

inline int find_vex(algraph *graph, elemtype *data)   // 查找结点地址
{
	int i;

	if (!graph) return NO;
	
	for (i=0; i<graph->vexnum; i++)
		if (graph->vex[i].data == *data) return i;
	return NO;
}

status insert_arc(algraph *graph, elemtype *st, elemtype *end)
{
	int flag;

	flag = insert_arc_s(graph, st, end);

	// 如果是无向图, 还要加多一条边,
	if (graph->kind == UDG)
		insert_arc_s(graph, end, st);
	return flag;
}

// 插入一条边, 类似链表插入一个结点
status insert_arc_s(algraph *graph, elemtype *st, elemtype *end)
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
	cur->info = NULL;
	cur->nextarc = graph->vex[addr].first;
	graph->vex[addr].first = cur;
	graph->arcnum++;

	return OK;
}
