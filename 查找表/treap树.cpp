#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

// treap 树, 二叉查找树的一种, 每个结点有优先级
// treap 树的元素具有查找树的性质, 优先级具有大顶堆的性质
// 优先级越大, 越靠近根, 越快找到, 即查找的次数少

#define OK 1
#define ERROR 0
typedef int status;
typedef int elemtype;

// treap 树结点结构
typedef struct node
{
	int p;			// 优先级
	elemtype data;	// 数据
	node *lchild;
	node *rchild;
}node, treap;


// 以 root 为根, 向右旋转, 和 AVL 树的旋转一样
inline void right(treap **root)
{
	node *lc = (*root)->lchild;;

	(*root)->lchild = lc->rchild;
	lc->rchild = *root;
	*root = lc;
}

// 向左旋转
inline void left(treap **root)
{
	node *rc = (*root)->rchild;

	(*root)->rchild = rc->lchild;
	rc->lchild = *root;
	*root = rc;
}

node *new_node(elemtype data, int p)	// 分配一个新结点
{
	node *n;

	n = (node *)malloc(sizeof(node));
	n->data = data;
	n->p = p;
	n->lchild = n->rchild = NULL;
	return n;
}

// 插入一个不存在的结点, 若存在也可重复插入, 但这里不插入重复的
// 按照二叉排序树的方法插入, 在回溯的时候, 不断旋转, 调整优先级
status insert(treap **tree, elemtype data, int p)
{
	status s;
	if (!(*tree)) 
	{
		*tree = new_node(data, p);
		return OK;
	}
	else if ((*tree)->data == data) return ERROR;	// 有了
	else if (data < (*tree)->data)
	{
		// 搜索左子树, 回溯的时候调整优先级
		// 回溯时, 发现左孩子的优先级大, 则向右旋转
		s = insert(&((*tree)->lchild), data, p);
		if ((*tree)->lchild->p > (*tree)->p)  right(tree);
	}
	else 
	{
		// 搜索右子树. 回溯时, 记得调整
		s = insert(&((*tree)->rchild), data, p);
		if ((*tree)->rchild->p > (*tree)->p) left(tree);
	}
	return s;
}

// 删除一个结点
// 先用用查找树的方法找到这个结点
// 然后用堆下调的方法, 把这个结点下调成叶子, 再释放掉
status remove(treap **tree, elemtype data)
{
	node *del;

	if (!(*tree)) return ERROR;

	if ((*tree)->data == data)	// 找到 data 进行下调
	{	
		// 一种巧妙的方法, 判断树的两个根的状态
		switch (!!((*tree)->lchild) + !!((*tree)->rchild) * 2)
		{
		case 0 :		// 叶子, 可以直接释放
			free(*tree);   *tree = NULL;   break;
		case 1 :			// 只有左孩子
			del = *tree;
			*tree = (*tree)->lchild;
			free(del);   break;
		case 2 :			// 只有右孩子
			del = *tree;
			*tree = (*tree)->rchild;
			free(del);   break;
		case 3 :		// 两个都有, 进行下调, 把优先级高的孩子浮上来
			if ((*tree)->lchild->p > (*tree)->rchild->p)
			{
				right(tree);	// 左边大, 向右旋转, 把左边的转上去
				remove(&((*tree)->rchild), data);
			}
			else	// 同时递归到左孩子, 进行删除
			{
				left(tree);
				remove(&((*tree)->lchild), data);
			}
		}
		return OK;
	}
	else if (data < (*tree)->data)
		return remove(&((*tree)->lchild), data);
	else return remove(&((*tree)->rchild), data);
}

// 检查优先级的满不满足堆序性
status check(treap *tree)
{
	int re = 1;

	if (!tree) return OK;
	if (tree->lchild)		// 有左孩子
	{
		if (tree->p < tree->lchild->p) return ERROR;
		if (!check(tree->lchild)) return ERROR;
		if (tree->rchild)		// 有右孩子
		{
			if (tree->p < tree->rchild->p) return ERROR;
			if (!check(tree->rchild)) return ERROR;
		}
	}
	else if (tree->rchild)		// 只有右孩子
	{
		if (tree->p < tree->rchild->p) return ERROR;
		if (!check(tree->rchild)) return ERROR;
	}
	return OK;
}

void destory(treap *tree)		// 销毁
{
	if (tree)
	{
		destory(tree->lchild);
		destory(tree->rchild);
		free(tree);
	}
}

void first_tra(treap *tree)		// 中序遍历
{
	if (tree)
	{
		printf("%d, ", tree->data);
		first_tra(tree->lchild);
		first_tra(tree->rchild);
	}
}

void mid_tra(treap *tree)		// 中序遍历
{
	if (tree)
	{
		mid_tra(tree->lchild);
		printf("%d, ", tree->data);
		mid_tra(tree->rchild);
	}
}

char display(treap *tree)	// 界面显示模块
{
	char c;

	if (check(tree)) printf("\ntreap 树正确!");
	else printf("\ntreap 树错误!");
	printf("\n\n先序遍历: ");   first_tra(tree);
	printf("\n中序遍历: ");   mid_tra(tree);
	
	printf("\n\n按 c 键结束, 按回车继续...\n");
	getchar();   c = getch();
	system("cls");

	return c;
}

int main(void)
{
	treap *tree = NULL;
	elemtype data;
	int p;

	while (1)		// 插入测试
	{
		printf("请输入要插入的元素及优先级: ");
		scanf("%d %d", &data, &p);
		if (insert(&tree, data, p))		// 插入
			printf("\n该元素不存在, 已插入!\n");
		else printf("\n%d 已存在!\n", data);

		if (display(tree) == 'c') break;
	}
	while (1)		// 删除测试
	{
		printf("请输入要删除的元素: ");
		scanf("%d", &data);
		if (!remove(&tree, data))		// 删除
			printf("\n该元素不存在, 删除失败!\n");
		else printf("\n已删除!\n");

		if (display(tree) == 'c') break;
	}
	destory(tree);
	return 0;
}
/*		测试数据, 结点 - 权值
1 10

2 8

3 6

4 11

5 0
*/