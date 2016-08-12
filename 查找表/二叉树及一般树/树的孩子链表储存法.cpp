// 树的孩子链表储存

#include <stdio.h>
#include <malloc.h>

typedef char elemtype;
typedef int status;

#define ERROR 0
#define OK 1
#define SIZE 30

// 孩子链表结点
typedef struct node
{
	int childaddr;	// 孩子的地址
	node *next;
}node;

// 头指针数组
typedef struct head
{
	elemtype data;	// 结点数值
	node *next;		// 头指针
}head;

// 头结构基址管理, 默认base[0]是根结点
typedef struct ctree
{
	head *base;		// 基址
	int cur;		// 使用量
	int size;		// 总大小
}ctree;

// 创建
status create(ctree *tree)
{
	int i;

	// 分配头结构空间
	tree->base = (head *)malloc(SIZE * sizeof(head));
	if (!tree->base) return ERROR;
	tree->size = SIZE;
	tree->cur = 0;

	// 初始化数组头指针
	for (i=0; i<SIZE; i++)
		tree->base[i].next = NULL;

	return OK;
}

// 寻找 c 结点的地址
head *find(ctree *tree, elemtype c)
{
	int i;

	if (!tree->base) return ERROR;
	for (i=0; i<tree->cur; i++)
		if (tree->base[i].data == c)
			return tree->base + i;
	return NULL;
}

// 赋值
status input(ctree *tree)
{
	int cur = 0;
	elemtype c;
	head *addr;		// 第i行的头结构指针
	node *n;		// 新孩子指针
	node *j;		// 尾指针

	if (!tree) return ERROR;
	// 建立头结构
	c = getchar();
	while (c != '\n')
	{
		// 看看满了没
		if (tree->cur == tree->size)
		{
			tree->size += 10;
			tree->base = (head *)realloc(tree->base, 
				tree->size * sizeof(head));
			if (!tree->base) return ERROR;
		}
		tree->base[tree->cur++].data = c;
		c = getchar();
	}

	// 遍历数组创建每个结点孩子
	fflush(stdin);
	while (cur < tree->cur)
	{
		printf("请输入 %c 的孩子：", tree->base[cur].data);
		// 创建当前结点的孩子
		c = getchar();
		while (c != '\n')
		{
			addr = find(tree, c);
			if (addr)
			{
				// 此时 addr 类似链表的头结构
				n = (node *)malloc(sizeof(node));
				if (!n) return ERROR;
				n->childaddr = addr - tree->base;
				
				// 尾插法，尾指针 j
				if (tree->base[cur].next)
				{
					j = tree->base[cur].next;
					while (j->next)
						j = j->next;
					n->next = j->next;
					j->next = n;
				}
				else 
				{
					n->next = tree->base[cur].next;
					tree->base[cur].next = n;
				}
			}
			c = getchar();
		}
		cur++;
	}
	return OK;
}

// 输出
status show(ctree *tree, int addr)
{
	if (!tree->base) return ERROR;
	printf("%c, ", tree->base[addr].data);
	return OK;
}

// 先序遍历 (递归法)
status first_tra(ctree *tree, int pos,
				 status (*visit)(ctree *tree, int addr))
{
	node *cur;
	static char f = 1;
	// 输出一次根结点
	if (f) {visit(tree, 0); f = 0;}

	// 如果头指针不是空
	if (tree->base[pos].next)
	{
		// 遍历孩子链表
		cur = tree->base[pos].next;
		while (cur)
		{
			visit(tree, cur->childaddr);
			// 递归搜索下一个孩子
			first_tra(tree, cur->childaddr, visit);
			cur = cur->next;	// 回溯时，把该层的cur下拨一位
		}
	}
	else return ERROR;
	return OK;
}

// 求树的深度
// 基本项
// 如果是空表，返回 1
// 如果不是空表，返回 最深子树+1 
int lenth(ctree *tree, int pos)
{
	node *cur;
	int deep;
	int max = -1;

	// 为什么空表返回 1 ?
	// 当前层是空表，深度是 0 
	// 但是退回到上一层就是 1 了
	// 相当于我告诉上一层，你现在深度是 1
	if (!tree->base[pos].next) return 1;
	
	// 遍历孩子链表找到最深子树
	cur = tree->base[pos].next;
	while (cur)
	{
		deep = lenth(tree, cur->childaddr);
		if (deep > max) max = deep;
		cur = cur->next;
	}
	// 当前层深度是 max, 再退回一层就是 max + 1
	return max + 1;
}

int main(void)
{
	ctree tree;

	if (!create(&tree)) return 0;
	printf("说明：按层次输入结点，以回车键结束\n");
	printf("请输入全部结点：");
	input(&tree);
	printf("\n先序遍历：");
	first_tra(&tree, 0, show);
	printf("\n树的深度：%d\n", lenth(&tree, 0));

	return 0;
}
