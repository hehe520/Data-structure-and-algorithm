#include <stdio.h>
#include <stdlib.h>
#include "mgraph.h"

/*
图的输入
第一行是顶点数，随后 n 个顶点名称
随后x行是 a 到 b 权值是 c
以 0 0 0 结束
对于图可以不写权值，网就要写
图
5
ABCDEF
a b
b c
0 0
网
5
12345
a b 3
c b 6
0 0 0
*/

// 输出结点名字
status showname(nametype *data)
{
	if (data) printf("%c, ", *data);
	else return ERROR;
	return OK;
}

int main(void)
{
	mgraph graph;
	int kind;
	int n, i;
	nametype vex[MAX_NUM];	// 点集
	nametype back;
	nametype data;
	int pos;

	printf("请输入图的类型：(0.DG, 1.UDG, 2.DN, 3.UDN):");
	scanf("%d", &kind);
	printf("请输入顶点个数:");
	scanf("%d", &n);
	fflush(stdin);
	printf("请输入顶点名称:");
	for (i=0; i<n; i++)
		scanf("%1c", vex + i);
	fflush(stdin);

	if (kind == DG || kind == UDG)
		printf("请输入边：(如 a b, 表示 a 到 b)\n");
	else printf("请输入弧：(如 a b c, 表示 a 到 b 的权为 c)\n");
	create(&graph, kind, n, vex);	// 创建图
	input(&graph);					// 输入图
	printf("输出邻接矩阵:\n");
	show(&graph);

	printf("\n取第i号顶点的名称：");
	scanf("%d", &pos);
	if (getvex(&graph, pos, &back))
		printf("找到：%c\n", back);
	else printf("未找到\n");

	printf("\n更新顶点名称, (如X A):");
	fflush(stdin);
	scanf("%c %c", &back, &data);
	if (putvex(&graph, &back, &data))
	{
		printf("更新成功\n");
		show(&graph);
	}
	else
		printf("找不到 %c\n", back);

	printf("\n返回X的第一个邻接点：");
	fflush(stdin);
	scanf("%c", &data);
	pos = firstadjvex(&graph, &data);
	if (pos != NOT_FOUND) 
		printf("第一个邻接点是：%c\n", graph.vexs[pos]);
	else printf("未找到\n");

	printf("\n返回X中Y的下一个邻接点：");
	fflush(stdin);
	scanf("%c %c", &back, &data);
	pos = nextadjvex(&graph, &back, &data);
	if (pos != NOT_FOUND) 
		printf("%c 的下一个邻接点是：%c\n", data, graph.vexs[pos]);
	else printf("未找到\n");

	printf("\n插入一个结点：");
	fflush(stdin);
	scanf("%c", &data);
	if (insertvex(&graph, &data))
		show(&graph);
	else printf("\n邻接矩阵已满");

	printf("\n删除一个结点：");
	fflush(stdin);
	scanf("%c", &data);
	if (deletevex(&graph, &data))
		show(&graph);
	else printf("找不到 %c\n", data);

	if (graph.kind == DG || graph.kind == UDG)
	{
		printf("\n插入一条边：");
		fflush(stdin);
		scanf("%c %c", &back, &data);
		if (insertarc(&graph, &back, &data, 0)) 
			show(&graph);
		else printf("插入失败\n");
	}
	else
	{
		printf("\n插入一条弧：");
		fflush(stdin);
		scanf("%c %c %d", &back, &data, &pos);
		if (insertarc(&graph, &back, &data, pos)) 
			show(&graph);
		else printf("插入失败\n");
	}

	printf("\n删除一条边：");
	fflush(stdin);
	scanf("%c %c", &back, &data);
	if (deletearc(&graph, &back, &data)) 
		show(&graph);
	else printf("删除失败\n");

	printf("\n深度优先搜索：");
	graph_DFS(&graph, showname);
	printf("\n");

	printf("\n深度优先搜索(非递归)：");
	graph_DFS_s(&graph, showname);
	printf("\n");

	printf("\n广度优先搜索：");
	graph_BFS(&graph, showname);
	printf("\n\n");

	destory(&graph);
	return 0;
}
