#include <stdio.h>
#include "adjlist.h"

// 输出第 n 个结点
void print(algraph *graph, int addr)
{
	printf("%c, ", graph->vex[addr].data);
}

int main(void)
{
	algraph graph;		// 邻接表
	elemtype vexs[NUM];
	short int kind;
	int n, i;
	char a, b;

	printf("图的邻接表\n");
	printf("请输入图的类型 (0.DG, 1.UDG, 2.DN, 3.UDN): ");
	scanf("%d", &kind);
	printf("请输入顶点集: ");
	scanf("%d", &n);   fflush(stdin);
	for (i=0; i<n; i++)
		scanf("%1c", vexs + i);

	create(&graph, kind, vexs, n);		// 创建图
	printf("请输入边集: ");
	input(&graph);		// 批量输入
	printf("邻接表如下: \n");
	show(&graph);		// 查看邻接表

	printf("\n插入一条边: ");
	fflush(stdin);   scanf("%c %c", &a, &b);
	if (graph.kind == DN || graph.kind == UDN)	// 如果带权
		scanf("%d", &n);
	else n = 0;
	insert_arc(&graph, &a, &b, n);
	show(&graph);
	printf("\n删除一条边: ");
	fflush(stdin);   scanf("%c %c", &a, &b);
	remove_arc(&graph, &a, &b);
	show(&graph);
	printf("\n插入一个点: ");
	fflush(stdin);   scanf("%c", &a);
	insert_vex(&graph, &a);
	show(&graph);
	printf("\n删除一个点: ");
	fflush(stdin);   scanf("%c", &a);
	remove_vex(&graph, &a);
	show(&graph);
	printf("\nX 的第一个邻接点: (X): ");
	fflush(stdin);   scanf("%c", &a);
	n = first_vex(&graph, &a);
	if (n != NO)
		printf("%c 的第一个邻接点: %c\n", a, graph.vex[n].data);
	else printf("未找到\n");
	printf("\nX 的下一个邻接点: (X A): ");
	fflush(stdin);   scanf("%c %c", &a, &b);
	n = next_vex(&graph, &a, &b);
	if (n != NO)
		printf("%c 的下一个邻接点: %c\n", a, graph.vex[n].data);
	else printf("未找到\n");

	printf("\n广度优先搜索: ");
	BFS(&graph, print);
	printf("\n深度优先搜索: ");
	DFS(&graph, print);
	printf("\n深搜(非递归): ");
	DFS2(&graph, print);
	printf("\n\n");

	destory(&graph);
	return 0;
}
