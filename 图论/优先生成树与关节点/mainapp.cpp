/* ----------------------------------
// 深度和广度优先生成树
// 采用 邻接表   储存图
// 采用 兄弟链表 储存树
//
// 主要函数:
// DFS(图, 树)   队列广搜, 生成树
// BFS(图, 树)   递归深搜, 生成树
// BFS2(图, 树)  非递归深搜, 生成树
// DFS_node()    求关节点
----------------------------------- */

#include "adjlist.h"	// 邻接表的头文件
#include <stdio.h>
#include <stdlib.h>

// 树的先序遍历
void first_tra(tree *t)
{
	if (t)
	{
		printf("%c, ", t->data);
		first_tra(t->child);
		first_tra(t->brother);
	}
}

// 释放树
void del(tree **t)
{
	if (*t)
	{
		del(&(*t)->child);
		del(&(*t)->brother);
		free(*t);
	}
}

// 树的层次遍历
// 因为树没有回路, 所以不用标记访问
void BFS_tree(tree *t)
{
	tree *queue[60];	// 假设空间足够
	tree *tmp;
	int rear = 0;
	int front = 0;

	//   t  ->  a ->  b -> ...
	//  /      /
	// c ->.. d -> ...    
	queue[rear] = t;		// 根结点入队
	rear = (rear + 1) % 60;
	while (rear != front)
	{
		tmp = queue[front];
		front = (front + 1) % 60;
		while (tmp)		// 遍历兄弟, 有孩子的全部入队
		{
			printf("%c, ", tmp->data);
			if (tmp->child)		// 首孩子入队记录
			{
				queue[rear] = tmp->child;
				rear = (rear + 1) % 60;
			}
			tmp = tmp->brother;
		}
	}
}

int main(void)
{
	tree *t = NULL;		// 优先生成树
	algraph graph;		// 邻接表
	char str[50] = {'\0'};	// 记录关节点
	elemtype vexs[NUM];
	short int kind;
	int n, i;

	printf("图的邻接表\n");
	printf("请输入图的类型 (0.有向图, 1.无向图): ");
	scanf("%d", &kind);
	printf("请输入顶点集: ");
	scanf("%d", &n);   fflush(stdin);
	for (i=0; i<n; i++)
		scanf("%1c", vexs + i);

	create(&graph, kind, vexs, n);		// 创建图
	printf("请输入边集: ");
	input(&graph);		// 批量输入

	DFS(&graph, &t);
	printf("\n先序遍历生成树: ");
	first_tra(t);
	del(&t);	t = NULL;

	DFS2(&graph, &t);
	printf("\n先序遍历生成树: ");
	first_tra(t);
	del(&t);	t = NULL;
	printf("   <- 非递归生成");

	BFS(&graph, &t);
	printf("\n层次遍历生成树: ");
	BFS_tree(t);
	del(&t);	t = NULL;

	printf("\n\n该图的关节点: ");
	DFS_node(&graph, str);
	if (*str == '\0') printf("强连通图, 没有关节点!\n");
	else 
		for (i=0; str[i]!='\0'; i++)
		{
			// 因为如果两棵以上的子树, 根会重复记录为关节点
			// 这里是去掉字符串中重复的元素
			// 对于数据量小, 直接用 O(n^2) 的算法快
			// 对于海量数据, 用哈希表去掉重复的快 O(n)
			for (n=i+1; str[n]!='\0'; n++)
				if (str[n] == str[i]) break;
			if (str[n] == '\0') printf("%c, ", str[i]);
		}
	printf("\n");

	destory(&graph);
	return 0;
}
