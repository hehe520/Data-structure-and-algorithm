#include <stdio.h>
#include "mgraph.h"

// 创建图
status create(mgraph *graph, int kind, int n, nametype *vex)
{
	switch (kind)
	{
	case DG  : return create_G(graph, DG, n, vex);
	case UDG : return create_G(graph, UDG, n, vex);
	case DN  : return create_N(graph, DN, n, vex);
	case UDN : return create_N(graph, UDN, n, vex);
	default : return ERROR;
	}
}

// 创建图
status create_G(mgraph *graph, graphkind kind, int n, nametype *vex)
{
	int i, j;

	// 合法检查
	if (!graph) return ERROR;
	if (n >= MAX_NUM || n <= 0) return ERROR;

	// 初始化各成员
	graph->kind = kind;
	graph->arcnum = 0;
	graph->vexnum = n;
	for (i=0; i<n; i++) graph->vexs[i] = vex[i];
	
	// 初始化邻接矩阵
	for (i=0; i<n; i++)
		for (j=0; j<n; j++)
		{
			// 图没有权值,0是不通
			graph->arcs[i][j].adj = 0;
			graph->arcs[i][j].info = NULL;
			// 不设附加信息了
		}
	return OK;
}

// 创建网
status create_N(mgraph *graph, graphkind kind, int n, nametype *vex)
{
	int i, j;

	// 合法检查
	if (!graph) return ERROR;
	if (n >= MAX_NUM || n <= 0) return ERROR;

	// 初始化成员
	graph->arcnum = 0;
	graph->kind = kind;
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
	if (graph->kind == DG || graph->kind == UDG)
	{
		while (1)   // 构造边集
		{
			fflush(stdin);
			scanf("%c %c", &a, &b);
			// scanf(info)  还可以输入附加信息
			if (a == '0' && b == '0') break;   // 同时为 0
			insertarc(graph, &a, &b, 0);
			if (graph->kind == UDG) insertarc(graph, &b, &a, 0);
			// 无向图对称点也要插入
		}
	}
	else 
	{
		while (1)   // 构造弧集
		{
			fflush(stdin);
			scanf("%c %c %d", &a, &b, &quan);
			if (a == '0' && b == '0') break;
			insertarc(graph, &a, &b, quan);
			if (graph->kind == UDN) insertarc(graph, &b, &a, quan);
		}
	}
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
	return NOT_FOUND;
}

// 输出邻接矩阵
status show(mgraph *graph)
{
	int i, j;
	int n;
	
	if (!graph) return ERROR;
	
	// 输出顶点名称
	n = graph->vexnum;
	printf("%4c", ' ');
	for (i=0; i<n; i++)
		printf("%4c", graph->vexs[i]);
	printf("\n");
	
	// 如果是图
	if (graph->kind == UDG || graph->kind == DG)
	{
		// 输出矩阵
		for (i=0; i<n; i++)
		{
			printf("%4c", graph->vexs[i]);
			for (j=0; j<n; j++)
			{
				printf("%4d", graph->arcs[i][j].adj);
			}
			printf("\n");
		}
	}
	else	// 如果是网 
	{
		for (i=0; i<n; i++)
		{
			printf("%4c", graph->vexs[i]);
			for (j=0; j<n; j++)
			{
				if (graph->arcs[i][j].adj == INF)
					printf("%4c", CHARINF);
				else printf("%4d", graph->arcs[i][j].adj);
			}
			printf("\n");
		}
	}
	return OK;
}

status destory(mgraph *graph)
{
	int i, j;

	if (!graph) return ERROR;
	if (graph->kind == UDG || graph->kind == DG)
	{
		for (i=0; i<graph->vexnum; i++)
			for (j=0; j<graph->vexnum; j++)
			{
				graph->arcs[i][j].adj = 0;
				graph->arcs[i][j].info = NULL;
			}
	}
	else 
	{
		for (i=0; i<graph->vexnum; i++)
			for (j=0; j<graph->vexnum; j++)
			{
				graph->arcs[i][j].adj = INF;
				graph->arcs[i][j].info = NULL;
			}
	}
	graph->arcnum = 0;
	for (i=0; i<graph->vexnum; i++)
		graph->vexs[i] = '\0';
	graph->vexnum = 0;
	graph->kind = DG;
	return OK;
}

// 返回第 pos 个顶点的名称
status getvex(mgraph *graph, int pos, nametype *back)
{
	if (pos > graph->vexnum || pos <= 0) return ERROR;
	*back = graph->vexs[pos-1];
	return OK;
}

// 将 old 的名称改为 data
status putvex(mgraph *graph, nametype *old, nametype *data)
{
	int addr;

	addr = find_vex(graph, old);
	if (addr == NOT_FOUND) return ERROR;
	graph->vexs[addr] = *data;
	return OK;
}

// 返回key结点的第一个邻接点
int firstadjvex(mgraph *graph, nametype *key)
{
	int addr;
	int i;

	if (!graph) return NOT_FOUND;
	addr = find_vex(graph, key);   // 找到 key 结点
	if (addr == NOT_FOUND) return NOT_FOUND;

	// 如果是图，找不是 0 的点
	if (graph->kind == DG || graph->kind == UDG)
	{
		for (i=0; i<graph->vexnum; i++)
			if (graph->arcs[addr][i].adj == 1)
				return i;
	}
	else   // 如果是网，找不是INF的点
	{
		for (i=0; i<graph->vexnum; i++)
			if (graph->arcs[addr][i].adj != INF)
				return i;
	}
	return NOT_FOUND;
}

// 返回key结点相对于data下一个邻接点
int nextadjvex(mgraph *graph, nametype *key, nametype *data)
{
	int addr;  // key 的位置
	int pos;   // data 的位置
	int i;

	// 先找到 key 结点
	if (!graph) return NOT_FOUND;
	addr = find_vex(graph, key);
	if (addr == NOT_FOUND) return NOT_FOUND;

	// 再找到 data 邻接点, 找不到也返回not found
	pos = find_vex(graph, data);
	if (pos == NOT_FOUND) return NOT_FOUND;

	// 如果是图
	if (graph->kind == DG || graph->kind == UDG)
	{
		// 找不到 data 加到这里来，少做一些判断
		if (graph->arcs[addr][pos].adj == 0) return NOT_FOUND;
		for (i=pos+1; i<graph->vexnum; i++)
			if (graph->arcs[addr][i].adj == 1)
				return i;
	}
	else 
	{
		if (graph->arcs[addr][pos].adj == INF) return NOT_FOUND;
		for (i=pos+1; i<graph->vexnum; i++)
			if (graph->arcs[addr][i].adj != INF)
				return i;
	}
	return NOT_FOUND;
}

// 插入新结点
status insertvex(mgraph *graph, nametype *data)
{
	int i, n;

	if (!graph) return ERROR;

	// 看看矩阵满了没有
	if (graph->vexnum + 1 == MAX_NUM) return ERROR;

	// 顶点集增加一个，顶点数也增加一个
	graph->vexs[graph->vexnum++] = *data;

	// 初始化新结点
	n = graph->vexnum;
	if (graph->kind == DG || graph->kind == UDG)
	{
		for (i=0; i<n; i++)
		{
			graph->arcs[i][n-1].adj = 0;
			graph->arcs[i][n-1].info = NULL;
			graph->arcs[n-1][i].adj = 0;
			graph->arcs[n-1][i].info = NULL;
		}
	}
	else 
	{
		for (i=0; i<n; i++)
		{
			graph->arcs[i][n-1].adj = INF;
			graph->arcs[i][n-1].info = NULL;
			graph->arcs[n-1][i].adj = INF;
			graph->arcs[n-1][i].info = NULL;
		}
	}
	return OK;
}

// 删除所有依附 data 的边
status deleteallarc(mgraph *graph, nametype *data)
{
	int addr;
	int i;

	addr = find_vex(graph, data);
	if (addr == NOT_FOUND) return ERROR;

	//  data 能到的全部抹掉
	// 能到 data 的全部抹掉
	if (graph->kind == DG || graph->kind == UDG)
	{
		// data 能到的
		for (i=0; i<graph->vexnum; i++)
			if (graph->arcs[addr][i].adj == 1)
				graph->arcs[addr][i].adj = 0;

		// 能到 data 的
		for (i=0; i<graph->vexnum; i++)
			if (graph->arcs[i][addr].adj == 1)
				graph->arcs[i][addr].adj = 0;
	}
	else 
	{
		for (i=0; i<graph->vexnum; i++)
			if (graph->arcs[addr][i].adj != INF)
				graph->arcs[addr][i].adj = INF;
		for (i=0; i<graph->vexnum; i++)
			if (graph->arcs[i][addr].adj != INF)
				graph->arcs[i][addr].adj = INF;
	}
	return OK;
}

// 删除结点
status deletevex(mgraph *graph, nametype *data)
{
	int addr;
	int i, j;

	if (!graph) return ERROR;
	
	// 抹掉所有邻接边
	addr = find_vex(graph, data);
	if (addr == NOT_FOUND) return ERROR;
	deleteallarc(graph, data);

	// 在矩阵中去掉 addr
	for (i=addr; i<graph->vexnum; i++)
	{
		graph->vexs[i] = graph->vexs[i+1];   // 移动点集
		for (j=0; j<graph->vexnum - 1; j++)	 // 移动边集
			graph->arcs[j][i].adj = graph->arcs[j][i+1].adj;
		for (j=0; j<graph->vexnum - 1; j++)
			graph->arcs[i][j].adj = graph->arcs[i+1][j].adj;
	}
	graph->vexnum--;
	return OK;
}

// 插入边
status insertarc(mgraph *graph, nametype *a, nametype *b, int quan)
{
	int x, y;

	if (!graph) return ERROR;
	x = find_vex(graph, a);
	y = find_vex(graph, b);
	if (x == NOT_FOUND || y == NOT_FOUND) return ERROR;

	if (graph->kind == DG || graph->kind == UDG)
	{
		graph->arcs[x][y].adj = 1;
		if (graph->kind == UDG) graph->arcs[y][x].adj = 1;
	}
	else 
	{
		graph->arcs[x][y].adj = quan;
		if (graph->kind == UDN) graph->arcs[y][x].adj = quan;
	}
	return OK;
}

// 删除边
status deletearc(mgraph *graph, nametype *a, nametype *b)
{
	int x, y;

	if (!graph) return ERROR;
	x = find_vex(graph, a);
	y = find_vex(graph, b);
	if (x == NOT_FOUND || y == NOT_FOUND) return ERROR;

	if (graph->kind == DG || graph->kind == UDG)
	{
		graph->arcs[x][y].adj = 0;
		if (graph->kind == UDG) graph->arcs[y][x].adj = 0;
	}
	else 
	{
		graph->arcs[x][y].adj = INF;
		if (graph->kind == UDN) graph->arcs[y][x].adj = INF;
	}
	return OK;
}

// 全局访问标记数组
unsigned short int used[MAX_NUM];

// 深度搜索 (递归)
// 对于一个图，深度搜索每一个连通分支，
status graph_DFS(mgraph *graph, status (*visit)(nametype *data))
{
	int i;

	for (i=0; i<graph->vexnum; i++) 
		used[i] = 0;		// 初始化标记数组, 0 未用

	for (i=0; i<graph->vexnum; i++)		// 遍历每一个连通分支
		if (!used[i]) DFS(graph, i, visit);
	return OK;
}

// 搜索单个连通分支
// 基本项：
// 如果 addr 合法，进行访问
status DFS(mgraph *graph, int addr, status (*visit)(nametype *data))
{
	int i;
	nametype data;

	if (!graph) return ERROR;
	if (addr >= 0 && addr < graph->vexnum)
	{
		data = graph->vexs[addr];
		used[addr] = 1;		// 标记已使用
		visit(&data);		// 访问
		
		// 访问 i 的第一个邻接点k, 访问相对于 k 的下一个邻接点
		for (i = firstadjvex(graph, &data);
			i != NOT_FOUND;
			i = nextadjvex(graph, &data, &(graph->vexs[i])))
		{
			// 继续搜索下一个未用邻接点
			if (!used[i]) DFS(graph, i, visit);
		}
	}
	else return ERROR;
	return OK;
}

// 非递归深度搜索, 栈
// 1. 找一个未访问结点入栈
// 2. 如果栈不空，把和栈顶有关的也压入栈，直到没得访问
// 3. 出栈一个，执行 2, 直到栈空
status graph_DFS_s(mgraph *graph, status (*visit)(nametype *data))
{
	nametype *stack[SIZE];
	nametype *addr;
	nametype *back;
	int top = 0;
	int i;

	// 初始化 used 数组
	for (i=0; i<graph->vexnum; i++) used[i] = 0;
	
	// 遍历点集，找一个没有访问的结点
	for (i=0; i<graph->vexnum; i++)
	{
		if (!used[i])
		{
			// i 访问, 入栈
			addr = &(graph->vexs[i]);
			visit(addr);
			used[i] = 1;
			stack[top++] = addr;   // 入栈记录路径

			// 如果栈不空
			while (top != 0)
			{
				// 一直找下去，直到没路走为止
				while (top != 0)
				{
					// 把和栈顶有关的结点压入栈
					addr = stack[top - 1];
					// 遍历 addr 的邻接点，寻找一个未用邻接点
					for (i = firstadjvex(graph, addr); 
						i != NOT_FOUND; 
						i = nextadjvex(graph, addr, graph->vexs+i))
					{
						if (!used[i])
						{
							back = &(graph->vexs[i]);
							visit(back);
							used[i] = 1;
							if (top + 1 == SIZE) return ERROR;
							stack[top++] = back;  // 入栈, 新栈顶为 back
							break;
						}
					}
					// i 已走到尽头
					if (i == NOT_FOUND) break;
				}
				// 走到了尽头，出栈后退一步
				if (top != 0) top--;
			} // while 
		}
	} // for
	return OK;
}

// 广度搜索，队列
status graph_BFS(mgraph *graph, status (*visit)(nametype *data))
{
	nametype *queue[SIZE];
	int rear = 0;
	int front = 0;
	int n, i;
	nametype *back;

	if (!graph) return ERROR;
	n = graph->vexnum;
	for (i=0; i<n; i++) used[i] = 0;	// 初始化标记数组

	// 随便找一个结点入队
	for (i=0; i<n; i++)
	{
		if (!used[i])   // 如果还有没有入队的
		{
			if ((rear+1)%SIZE == front) return ERROR;   // 队列满了
			queue[rear] = &(graph->vexs[i]);
			rear = (rear + 1) % SIZE;
			used[i] = 1;		// 标记已使用
			while (rear != front)
			{
				// 出队访问
				back = queue[front];
				front = (front + 1) % SIZE;
				visit(back);

				// 与 back 有关联的全部入队
				for (i = firstadjvex(graph, back);
					i != NOT_FOUND; 
					i = nextadjvex(graph, back, graph->vexs + i))
				{
					if (!used[i]) 
					{
						if ((rear+1) % SIZE == front) return ERROR;
						queue[rear] = graph->vexs + i;
						rear = (rear + 1) % 100;
						used[i] = 1;
					}
				} // for
			} // while
		} // if
	} // for
	return OK;
}
