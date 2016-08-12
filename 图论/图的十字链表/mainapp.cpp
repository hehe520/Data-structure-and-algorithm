#include <stdio.h>
#include "olist.h"

int main(void)
{
	olgraph graph;		// 十字链表
	elemtype vexs[NUM];
	int n, i;
	char a, b;

	printf("图的十字链表\n");
	printf("请输入顶点集: ");
	scanf("%d", &n);   fflush(stdin);
	for (i=0; i<n; i++) scanf("%1c", vexs + i);

	create(&graph, vexs, n);		// 创建图
	printf("请输入边集: ");
	input(&graph);		// 批量输入
	printf("十字链表部分关系如下: \n");
	show(&graph);		// 查看十字链表

	printf("\n插入一条边: ");
	fflush(stdin);   scanf("%c %c", &a, &b);
	insert_arc(&graph, &a, &b);
	show(&graph);
	printf("\n删除一条边: ");
	fflush(stdin);   scanf("%c %c", &a, &b);
	remove_arc(&graph, &a, &b);
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

	destory(&graph);
	return 0;
}
