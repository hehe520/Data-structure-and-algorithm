// 线索二叉树，中序线索

#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>

typedef int status;
typedef char elemtype;

#define ERROR 0
#define OK 1

typedef enum {link, thread} tag;
// link 是孩子，thread 是线索

// 线索二叉树储存结构
typedef struct btnode
{
	elemtype data;
	btnode *lchild;
	btnode *rchild;
	tag ltag;
	tag rtag;
}btnode;

// 根据str来创建树，返回地址
char *create(btnode **tree, char *str)
{
	if (*str != '\0')
	{
		if (*str == '#') *tree = NULL;
		else
		{
			*tree = (btnode *)malloc(sizeof(btnode));
			if (!(*tree)) exit(0);
			(*tree)->data = *str;
			(*tree)->lchild = NULL;
			(*tree)->rchild = NULL;
			str = create(&((*tree)->lchild), str + 1);
			str = create(&((*tree)->rchild), str + 1);
		}
	}
	return str;
}

status destory(btnode *tree)
{
	if (tree->ltag == link || tree->ltag == link)
	{
		destory(tree->lchild);
		destory(tree->rchild);
		free(tree);
	}
	return OK;
}

// ----------中序线索树部分---------------

// 非递归，中序遍历，
status mid_tra_s(btnode *tree)
{
	btnode *stack[150] = {0};
	int top = 0;
	btnode *back;
	
	while (top || tree)
	{
		// 一直往左走到尽头
		while (tree)
		{
			stack[top++] = tree;
			tree = tree->lchild;
		}
		// 每次往回走一个，看看有没有右孩子
		if (top)
		{
			back = stack[--top];
			printf("%c ", back->data);
			tree = back->rchild;
		}
	}
	return OK;
}

// 生成线索树
// 中序线索，递归部分
btnode *threading(btnode *tree, btnode *pre)
{
	if (tree)
	{
		pre = threading(tree->lchild, pre);
		// 基本项
		// 如果左树空闲，左树保存前驱信息
		if (!tree->lchild)
		{
			tree->lchild = pre;
			tree->ltag = thread;
		}
		else tree->ltag = link;
		// 如果双亲的右孩子空闲，则保存后继信息，
		if (!pre->rchild)
		{
			pre->rchild = tree;
			pre->rtag = thread;
		}
		else pre->rtag = link;
		pre = tree;		// 相当于 pre 向孩子走一步
		pre = threading(tree->rchild, pre);
	}
	return pre;
}

// 中序线索化
status threadtree(btnode *th, btnode *tree)
{
	btnode *pre;	// 记录上一个访问的结点

	if (!tree) return ERROR;
	th->ltag = link;
	th->lchild = tree;
	th->rtag = thread;
	th->rchild = th;	// 右指针回指
	
	pre = th;
	pre = threading(tree, pre);
	// 最后一个结点线索
	pre->rchild = th;
	pre->rtag = thread;
	th->rchild = pre;

	return OK;
}

// 中序线索遍历
status mid_tra_th(btnode *th)
{
	btnode *cur;

	cur = th->lchild;
	// 类似循环链表的结束标志，cur != head
	while (cur && cur != th)
	{
		// 中序遍历，左根右
		while (cur->ltag == link) 
			cur = cur->lchild;
		printf("%c ", cur->data);
		// 根据后继线索，不断回去，
		while (cur->rtag == thread && cur->rchild != th)
		{
			cur = cur->rchild;
			printf("%c ", cur->data);
		}
		cur = cur->rchild;
	}
	return OK;
}

// ------------非递归遍历部分--------------
// 先序遍历
status first_tra_s(btnode *tree)
{
	btnode *stack[150] = {0};
	int top = 0;
	btnode *back;

	while (top != 0 || tree)
	{
		while (tree)
		{
			printf("%c ", tree->data);
			stack[top++] = tree;
			tree = tree->lchild;
		}
		if (top != 0)
		{
			back = stack[--top];
			tree = back->rchild;
		}
	}
	return OK;
}

// 后续非递归遍历
status last_tra_s(btnode *tree)
{
	btnode *stack[150] = {0};
	int top = 0;
	btnode *pre;

	stack[top++] = tree;	// 根入栈
	tree = tree->lchild;
	while (top != 0)
	{
		// 把栈顶的左孩子全部进栈
		tree = stack[top-1];
		while (tree->lchild)
		{
			stack[top++] = tree->lchild;
			tree = tree->lchild;
		}
		// 找到一个可以向右走的结点入栈
		pre = stack[top-1];
		while (top != 0)
		{
			tree = stack[top-1];
			if (tree->rchild==NULL || tree->rchild == pre) 
			{
				printf("%c ", tree->data);
				top--;
				pre = tree;
			}
			else 
			{
				stack[top++] = tree->rchild;
				break;
			}
		}
	}
	return OK;
}

// 先序线索化递归处理部分
btnode *fthreading(btnode *tree, btnode *pre)
{
	if (tree)
	{
		// 保存前驱信息
		if (!tree->lchild)
		{
			tree->lchild = pre;
			tree->ltag = thread;
		}
		else tree->ltag = link;

		// 保存后继信息
		if (!pre->rchild)
		{
			pre->rchild = tree;
			pre->rtag = thread;
		}
		pre = tree;
		if (tree->ltag != thread)
			pre = fthreading(tree->lchild, pre);
		if (tree->rtag != thread)
			pre = fthreading(tree->rchild, pre);
	}
	return pre;
}

// 先序线索化树
status fthreadtree(btnode *th, btnode *tree)
{
	btnode *pre;

	if (!tree) return ERROR;

	// 初始化线索头
	th->lchild = tree;
	th->ltag = link;
	th->rchild = th;
	th->rtag = thread;
	pre = th;

	pre = fthreading(tree, pre);
	pre->rchild = th;
	pre->rtag = thread;
	th->rchild = pre;
	return OK;
}

// 遍历先序线索树
// 如果左孩子不是线索就一直走下去
// 然后向右线索走一步
status first_tra_th(btnode *th)
{
	btnode *cur;

	if (!th->lchild) return ERROR;
	cur = th->lchild;
	while (cur && cur != th)
	{
		// 按照指针走完左边
		while (cur && cur->ltag == link)
		{
			printf("%c ", cur->data);
			cur = cur->lchild;
		}
		// 按照线索回去，碰到 th 结束
		while (cur && cur->rtag == thread && cur != th)
		{
			printf("%c ", cur->data);
			cur = cur->rchild;
		}
	}

	return OK;
}

int main(void)
{
	btnode *tree;
	btnode th;		// 中序线索头
	btnode fth;		// 先序线索头
	char buf[200] = {0};

	// -+a##*b##-c##d##/e##f##
	printf("建立线索二叉树：");
	gets(buf);
	create(&tree, buf);
	
	printf("\n普通后序遍历：");
	last_tra_s(tree);

	printf("\n\n普通先序遍历：");
	first_tra_s(tree);
	printf("\n");
	fthreadtree(&fth, tree);	// 先序线索化
	printf("线索先序遍历：");
	first_tra_th(&fth);			// 线索遍历
	printf("\n");
	destory(tree);
	tree = NULL;

	create(&tree, buf);
	printf("\n普通中序遍历：");
	mid_tra_s(tree);
	printf("\n");
	threadtree(&th, tree);		// 中序线索化
	printf("线索中序遍历：");
	mid_tra_th(&th);			// 线索遍历
	printf("\n\n");
	destory(tree);
	tree = NULL;

	return 0;
}

