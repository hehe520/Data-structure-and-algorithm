// 二叉树层次建树，

#include <stdio.h>
#include <malloc.h>

typedef char elemtype;
typedef int status;

#define OK 1
#define ERROR 0

// 二叉树储存结构
typedef struct node
{
	elemtype data;
	node *lchild;
	node *rchild;
}node, btree;

// 层次建树
status create(btree **tree, char *str)
{
	node *queue[100];
	int rear = 0;
	int front = 0;
	node *back;

	if (str[0] == '\0' || str[0] == '#') return ERROR;
	
	// 建立根结点, 入队记录
	(*tree) = (node *)malloc(sizeof(node));
	if (!(*tree)) return ERROR;
	(*tree)->data = *str++;
	(*tree)->lchild = NULL;
	(*tree)->rchild = NULL;
	queue[rear] = *tree;
	rear = (rear + 1) % 100;

	while (rear != front)
	{
		back = queue[front];
		front = (front + 1) % 100;

		// 还有字符的时候，继续建树
		if (*str != '\0')
		{
			// 因为在创建的时候，就已经将孩子的
			// 指针初始化成 NULL 了
			// 所以 str == '#' 的情况可以不再赋值了 
			if (*str != '#')
			{
				// 建立左孩子
				back->lchild = (node *)malloc(sizeof(node));
				if (!back->lchild) return ERROR;

				back->lchild->data = *str;
				back->lchild->lchild = NULL;
				back->lchild->rchild = NULL;
				// 别忘记，入队记录
				queue[rear] = back->lchild;
				rear = (rear + 1) % 100;
			}
			str++;

			if (*str != '#')
			{
				// 建立右孩子
				back->rchild = (node *)malloc(sizeof(node));
				if (!back->rchild) return ERROR;

				back->rchild->data = *str;
				back->rchild->lchild = NULL;
				back->rchild->rchild = NULL;
				queue[rear] = back->rchild;
				rear = (rear + 1) % 100;
			}
			str++;
		}
	}
	return OK;
}

// 先序遍历
status first_tra(btree *tree)
{
	printf("%c, ", tree->data);
	if (tree->lchild)
		first_tra(tree->lchild);
	if (tree->rchild)
		first_tra(tree->rchild);
	return OK;
}

int main(void)
{
	btree *tree;
	char buffer[100] = {0};

	// 两组测试数据
	// ABECDFGH###J##I######
	// AB#CD##EF#G##
	printf("层次建立二叉树: ");
	scanf("%s", buffer);
	create(&tree, buffer);
	printf("\n先序遍历: ");
	first_tra(tree);
	printf("\n");

	return 0;
}