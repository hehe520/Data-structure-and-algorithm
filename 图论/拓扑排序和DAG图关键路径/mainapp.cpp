// 有向无环图 - 拓扑排序和关键路径
// 主要函数
// tsort()     拓扑排序
// cpath()     关键活动
// DFS_show()  深搜,输出关键路径

#include <stdio.h>
#include <malloc.h>
#include "adjlist.h"	// 邻接表储存图


// 求得各个结点入度, 保存到 back
void init_degree(algraph *graph, int back[])
{
	int i;
	vnode *cur;
	for (i=0; i<graph->vexnum; i++) back[i] = 0;

	for (i=0; i<graph->vexnum; i++)		// 遍历邻接表所有结点
	{
		cur = graph->vex[i].first;		// 当前的邻接点
		while (cur)
		{
			back[cur->adj]++;		// 可达结点的入度 +1
			cur = cur->nextarc;
		}
	}
}

// ---------------------拓扑排序------------------------
// 记录入度为 0 的结点, 用这些结点去消减其他邻接点的入度
// 直到队列空了, 若结点没有全部访问, 则存在环, 
// 环中没有入度为 0 的结点
status tsort(algraph *graph, char *back)
{
	int *indegree;		// 记录每个结点的入度
	int count = 0;		// 记录访问的结点数
	int queue[40];		// 队列记录
	int rear = 0;
	int front = 0;
	vnode *cur;
	int i, tmp;

	indegree = (int *)malloc(graph->vexnum * sizeof(int));
	init_degree(graph, indegree);		// 初始化每个结点的入度

	for (i=0; i<graph->vexnum; i++)		// 记录入度为 0 的结点
	{
		if (!indegree[i]) 
		{
			queue[rear] = i;
			rear = (rear + 1) % 40;
		}
	}
	while (rear != front)
	{
		tmp = queue[front];
		front = (front + 1) % 40;
		count++;		// 访问个数++
		*back++ = graph->vex[tmp].data;		// 记录一个拓扑序列
		cur = graph->vex[tmp].first;
		while (cur)		// 遍历邻接点
		{
			indegree[cur->adj]--;	// 消减邻接点的入度
			if (indegree[cur->adj] == 0)	// 减到 0 了也入栈
			{
				queue[rear] = cur->adj;
				rear = (rear + 1) % 40;
			}
			cur = cur->nextarc;
		}
	}
	// 没有访问到全部结点, 说明有环
	if (count < graph->vexnum) return ERROR;
	*back = '\0';
	return OK;
}
// -----------------end 拓扑排序---------------------


// -------------------关键路径-----------------------
void output(char *str)
{
	int i;
	printf("(");
	for (i=0; str[i]!='\0'; i++)
	{
		printf("%c", str[i]);
		if (str[i+1] != '\0') printf(", ");
	}
	printf("), ");
}

// 关键路径递归部分, path 是一条关键路径, flag 是关键活动
void DFS(algraph *graph, 
		 char *path,	// 关键路径
		 char *flag,	// 关键活动标志
		 int start,		// 开始点
		 int top,		// top 是 path 记录位置
		 int lens,		// lens 是关键路径长度
		 int len)		// len 目前的长度
{	
	vnode *cur = graph->vex[start].first;

	path[top] = graph->vex[start].data;		// 记录一个点

	// 如果没有邻接点, 表示到了完成点, 结束递归, 并输出
	if (!cur)
	{
		if (len == lens)		// 最长的路径才是关键路径, 否则不是
		{
			path[top + 1] = '\0';
			output(path);		// 用一张表来保存也可
		}
	}
	else 
	{
		while (cur)		// 遍历状态树
		{
			if (flag[cur->adj])		// 标记的活动是关键活动
			{
				top += 1;   len += cur->wight;
				DFS(graph, path, flag, cur->adj, top, lens, len);
				top -= 1;   len -= cur->wight;
				// 回溯的时候, 要恢复原来的值, 搜索下一条路径
			}
			cur = cur->nextarc;
		}
	}
}

// 深搜, 输出所有关键路径, 
// 关键路径上的活动是关键活动,
// 但关键活动组成的路径不一定是关键路径, 
// 要最长的 n 条才是关键路径, 参数中 max_path 就是标准长度了
void DFS_show(algraph *graph, char *flag, int max_path)
{
	int indegree[NUM];
	int i;
	char path[NUM + 10];
	
	init_degree(graph, indegree);
	for (i=0; i<graph->vexnum; i++)
	{ if (!indegree[i]) break; }
	// 此时 i 是开始点, 开始点只有一个
	DFS(graph, path, flag, i, 0, max_path, 0);
}

// 拓扑检测环, 顺便求活动的最早发生时间 ve, 及拓扑逆序列
status tsort_early(algraph *graph, int *ve, int *stack, int *top)
{
	int indegree[NUM];	// 记录入度
	int queue[60];
	int rear = 0;
	int front = 0;
	int count = 0;	// 统计访问顶点个数
	int i, tmp;
	vnode *cur;

	init_degree(graph, indegree);	// 获得各个顶点入度
	for (i=0; i<graph->vexnum; i++)
	{
		ve[i] = 0;			// 初始化最早发生时间
		if (!indegree[i])	// 入度为 0 的记录
		{	
			queue[rear] = i;
			rear = (rear + 1) % 60;
		}
	}
	while (rear != front)
	{
		tmp = queue[front];
		front = (front + 1) % 60;
		count++;
		stack[(*top)++] = tmp;		// 记录拓扑序列
		cur = graph->vex[tmp].first;
		while (cur)
		{
			// 此时边的方向是 tmp 至 cur->adj
			indegree[cur->adj]--;	// 消减可达结点的入度
			if (indegree[cur->adj] == 0)
			{
				queue[rear] = cur->adj;
				rear = (rear + 1) % 60;
			}
			// 求最早发生时间, 如 v -> w 是 wight 时间
			// 递推公式 ve[w] = max{ ve[v] + wight }
			if (ve[cur->adj] < ve[tmp] + cur->wight) 
				ve[cur->adj] = ve[tmp] + cur->wight;
			cur = cur->nextarc;
		}
	}
	if (count < graph->vexnum) return ERROR;	// 有环
	return OK;
}

// 输出关键活动到 flag
// 要找关键活动, 就必须找出最早最迟发生时间相等的结点
status cpath(algraph *graph, char *flag)
{
	int *early;		// 最早发生时间
	int *later;		// 最迟发生时间 (不影响关键路径时,最迟的发生时间)
	int stack[100];   // 记录拓扑逆序列, 求最迟发生时间
	int top = 0;
	int max = -1;
	int i, tmp;
	vnode *cur;

	early = (int *)malloc(graph->vexnum * sizeof(int));
	later = (int *)malloc(graph->vexnum * sizeof(int));
	if (!(early && later)) return ERROR;

	// 检测环, 求最早发生时间到 early
	if (!tsort_early(graph, early, stack, &top)) 
	{
		free(early);   free(later);
		return ERROR;
	}
	// 完成点只有一个, 找到最早发生时间最大的那个是完成点,
	for (i=graph->vexnum-1; i>=0; i--) 
	{ if (early[i] > max) max = early[i]; }
	for (i=0; i<graph->vexnum; i++) later[i] = max;	// 初始化最迟时间

	// 求最迟发生时间, 此时 early, 逆序已求好
	// w <- v 时间是 wight, 递推公式 vl[w] = min{ vl[v] - wight }		
	while (top)
	{
		tmp = stack[--top];
		cur = graph->vex[tmp].first;
		// 遍历邻接点, 寻找被松弛的对象, 此时 tmp 到邻接点 cur 是 wight
		// 不用担心 vl[tmp] 没有求出, 根据拓扑排序的性质
		// vl[cur->adj] 在 vl[tmp] 之前已求出
		while (cur)
		{
			// 因为这里是逆序操作, 和以前相反
			// tmp 是出发点, 也是要求 later 的点, tmp -> cur
			// 根据 cur 来更新自己
			if (later[tmp] > later[cur->adj] - cur->wight)
				later[tmp] = later[cur->adj] - cur->wight;
			cur = cur->nextarc;
		}
	}
	// 找最早最迟时间相等的活动即是关键活动, 标记为 1
	for (i=0; i<graph->vexnum; i++)
	{
		if (early[i] == later[i]) flag[i] = 1;
		else flag[i] = 0;
	}
	free(early);   free(later);
	return max;
}

int main(void)
{
	algraph graph;		// 邻接表
	char str[70];
	elemtype vexs[NUM];
	char flag[NUM + 5];   // 记录关键活动
	int max_path;		  // 关键路径长度
	short int kind;
	int n, i;

	printf("有向无环图 - 拓扑排序和关键路径\n");
	printf("请输入图的类型 (0.有向图, 1.有向网): ");
	scanf("%d", &kind);
	printf("请输入顶点集: ");
	scanf("%d", &n);   fflush(stdin);
	for (i=0; i<n; i++) scanf("%1c", vexs + i);
	create(&graph, kind, vexs, n);		// 创建图
	printf("请输入边集: ");
	input(&graph);		// 批量输入

	if (graph.kind == DG)	// 进行拓扑排序
	{
		if (tsort(&graph, str))
		{
			printf("\n拓扑序列: ");
			for (i=0; str[i]!='\0'; i++)
				printf("%c, ", str[i]);
		}
		else printf("错误: 该图存在环!");
	}
	else	// 进行关键路径
	{
		max_path = cpath(&graph, flag);
		if (max_path)
		{
			printf("\n关键活动: ");
			for (i=0; i<graph.vexnum; i++)
			{	if (flag[i])  printf("%c, ", graph.vex[i].data); }
			printf("\n关键路径: ");
			DFS_show(&graph, flag, max_path);	// 输出关键路径
			printf("\n路径长度: %d", max_path);
		}
		else printf("错误: 该图存在环!");
	}
	printf("\n");
	destory(&graph);
	return 0;
}
