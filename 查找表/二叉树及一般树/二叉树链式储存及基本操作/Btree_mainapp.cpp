// 二叉树的链式储存及相关操作

#include <stdio.h>
#include "Btree.h"

// 自定义操作
status show(btnode *tree)
{
	if (!tree) return ERROR;
	printf("%-2c", tree->data);
	return OK;
}

int main(void)
{
	btnode *tree = NULL;   // 保存根结点的地址
	char buf[100] = {0};
	char c, n, d;
	btnode *addr;

	printf("先序建树：(# 号表示空)");
	create(&tree);      // 直接输入字符串
//	gets(buf);			// 根据 buf 来建树
//	create2(&tree, buf);  //ABC##DE#G##F###
	init_parent(tree);	// 初始化双亲, 应放在create函数里面
	
	printf("\n先序遍历：");
	first_tra(tree, show);
	printf("\n非递归法：");
 	first_tra_s(tree, show);

	printf("\n\n中序遍历：");
	mid_tra(tree, show);
	printf("\n非递归法：");
	mid_tra_s(tree, show);

	printf("\n\n后序遍历：");
	last_tra(tree, show);
	printf("\n非递归法：");
	last_tra_s(tree, show);
	
	printf("\n\n层次遍历：");
	wild_tra(tree, show);

	printf("\n\n树的深度：%d\n", depth(tree));
	printf("结点数：%d\n", nodenum(tree));
	printf("叶子数：%d\n\n", leafnum(tree));

	printf("请输入要查找的结点：");
	fflush(stdin);
	scanf("%c", &c);
	addr = find(tree, &c);
	if (addr) 
	{
		printf("找到, 地址为: %x\n", addr);
		if (addr->parent)
			printf("其双亲为 %c\n", addr->parent->data);
		addr = root(addr);
		printf("根结点为：%c\n", addr->data);
	}
	else 
		printf("没有找到!\n");

	printf("\n输入要修改的结点和新值:(如A T) ");
	fflush(stdin);
	scanf("%c %c", &c, &n);
	if (update(tree, &c, &n))
	{
		printf("修改成功!\n");
		printf("修改后：");
		wild_tra(tree, show);
	}
	else printf("找不到 %c 结点", c);
	printf("\n");

	printf("\n在X结点下插入结点新值和位置(如 X A 0)");
	fflush(stdin);
	scanf("%c %c %c", &c, &n, &d);
	if (insert(tree, &c, &n, d))
	{
		printf("插入成功!\n");
		printf("插入后：");
		wild_tra(tree, show);
		printf("\n插入后树的深度：%d", depth(tree));
	}
	else printf("当前位置不可插入!");
	printf("\n");

	printf("\n请输入要删除结点的孩子(如 A 1)");
	fflush(stdin);
	scanf("%c %c", &c, &d);
	if (remove(tree, &c, d))
	{
		printf("删除后：");
		wild_tra(tree, show);
		printf("\n删除后树的深度：%d\n", depth(tree));
	}
	else printf("删除的位置不正确!\n");

	destory(tree);

	return 0;
}
