
// 二叉查找树的相关操作

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

typedef int elemtype;
typedef int status;
#define OK 1
#define ERROR 0

// 二叉链表储存
typedef struct bnode
{
	elemtype data;
	bnode *lchild;
	bnode *rchild;
}btree, bnode;


// 查找一个元素 data, 用 back 返回地址, p 是 tree 的双亲
status search(btree **tree, btree *p, elemtype data, bnode **back)
{
	if (!(*tree)) 
	{
		*back = p;		// 找不到就返回最近的双亲
		return ERROR;	// 方便插入操作
	}
	else if ((*tree)->data == data) return OK;
	else if (data < (*tree)->data) 
		return search( &((*tree)->lchild), *tree, data, back);
	else 
		return search( &((*tree)->rchild), *tree, data, back);
	// 这里直接返回子树的值
}

// 插入一个不存在的元素, 若存在返回 OK
status insert(btree **tree, elemtype data)
{
	bnode *add;		// 待插入结点双亲
	bnode *n;

	if (!search(tree, NULL, data, &add))
	{
		// 若不存在这个元素就插入 
		n = (bnode *)malloc(sizeof(bnode));
		n->data = data;
		n->lchild = n->rchild = NULL;

		// add 是待插入结点的上一个结点  add -> new_node
		if (!add) *tree = n;
		else
		{
			if (data < add->data)
				add->lchild = n;
			else add->rchild = n;
		}
		return ERROR;
	}
	else return OK;
}

// 删除一个结点, 算法1: 重接子树法
// 删除一个结点 cur, 左右都不空时
// 删除 cur 时,  cur 的左孩子由 f 继承
//  cur 的右孩子, 接到左孩子的最右边
status del(bnode *f, bnode **node)
{
	bnode *cur = *node;		// cur 待删除的结点

	if (!cur->rchild)	// 右子树空
	{
		// (!f) 没有双亲的结点就是根
		if (!f) *node = cur->lchild;
		else if (f->lchild == cur)  f->lchild = cur->lchild;
		else f->rchild = cur->lchild;
	}
	else if (!cur->lchild)	// 左子树空
	{
		if (!f) *node = cur->rchild;
		else if (f->lchild == cur)  f->lchild = cur->rchild;
		else f->rchild = cur->rchild;
	}
	else	// 左右都不空, 算法见上
	{
		// 把右子树接在左子树的最右边 (中序性质)
		cur = (*node)->lchild;
		while (cur->rchild)  cur = cur->rchild;
		cur->rchild = (*node)->rchild;		// 重接右子树

		cur = *node;
		if (!f) *node = cur->lchild;	// 删的是根, 直接在根上重接
		else if (f->lchild == cur) 
			f->lchild = cur->lchild;	// f 左孩子继承左子树
		else 
			f->rchild = cur->lchild;	// f 右孩子继承左子树
	}
	free(cur);		// 释放掉 cur
	return OK;
}

// 删除一个结点, 算法2: 前驱替换法
// 让前驱来自己的位置, 释放前驱结点
// 左右都不为空时, 前驱结点必定没有右子树 (中序性质)
status del2(bnode *f, bnode **node)
{
	bnode *pre;		// 前驱结点
	bnode *fpre;	// 前驱的双亲
	bnode *cur = *node;

	if (!cur->lchild)		// 左子树空
	{
		if (!f) *node = cur->rchild;
		else if (f->lchild == cur) f->lchild = cur->rchild;
		else f->rchild = cur->rchild;
	}
	else if (!cur->rchild)	// 右子树空
	{
		if (!f) *node = cur->lchild;
		else if (f->lchild == cur) f->lchild = cur->lchild;
		else f->rchild = cur->lchild;
	}
	else	// 左右都不空
	{
		pre = cur->lchild;		// pre 找前驱
		while (pre->rchild)  
		{
			fpre = pre;		// 记录双亲结点, 便于释放 pre
			pre = pre->rchild;
		}
		cur->data = pre->data;		// 前驱替换到 node
		if (pre == cur->lchild)  
			cur->lchild = pre->lchild;	// 类似右子树空的重接
		else fpre->rchild = pre->lchild;
		// 此时 pre 没有右子树, 只需重接左子树
		cur = pre;   // pre 是待释放的结点, 给下面释放
	}
	free(cur);
	return OK;
}

// 查找一个待删除结点, f 是双亲指针
// 因为要直接在树上删除, 要么传指针的指针, 要么传指针的引用
status remove(btree *f, btree **tree, elemtype data)
{
	if (!(*tree)) return ERROR;
	else if ((*tree)->data == data)
	{
		return del(f, tree);
//		return del2(f, tree);
//		del() 和 del2() 是两种删除算法, 都可使用
	}
	else if (data < (*tree)->data)
		return remove(*tree, &((*tree)->lchild), data);
	else
		return remove(*tree, &((*tree)->rchild), data);
}

// 递归求 ASL, num 结点个数, len 总长度, flood 递归层数
void ASL_s(btree *tree, int *num, double *len, int flood)
{
	if (!tree) return ;

	(*num)++;   (*len) += flood;
	// 累计完当前层的, 继续向下递归
	ASL_s(tree->lchild, num, len, flood + 1);
	ASL_s(tree->rchild, num, len, flood + 1);
}

double ASL(btree *tree)		// 计算平均查找长度 ASL
{
	int num = 0;
	double len = 0.0;

	ASL_s(tree, &num, &len, 1);
	if (num == 0) return 0;
	else return len / num;
}

int depth(btree *tree)		// 求深度
{
	int a, b;

	if (!tree) return 0;

	a = depth(tree->lchild);
	b = depth(tree->rchild);

	return (a > b ? a : b) + 1;
}

void mid_tra(btree *tree)	// 中序遍历
{
	if (tree)
	{
		mid_tra(tree->lchild);
		printf("%d, ", tree->data);
		mid_tra(tree->rchild);
	}
}

void destory(btree *tree)	// 销毁树
{
	if (tree)
	{
		destory(tree->lchild);
		destory(tree->rchild);
		free(tree);
	}
}

char display(btree *tree)	// 界面显示模块
{
	char c;

	printf("\n中序遍历: ");
	mid_tra(tree);
	printf("\n\n平均查找长度: %.4lf\t\t", ASL(tree));
	printf("树的深度: %d\n", depth(tree));
	
	printf("\n按 c 键结束, 按回车继续...\n");
	getchar();    c = getch();
	system("cls");

	return c;
}

int main(void)
{
	btree *tree = NULL;
	int data;

	while (1)		// 插入测试
	{
		printf("请输入要查找的元素: ");
		scanf("%d", &data);
		if (!insert(&tree, data))		// 插入
			printf("\n该元素不存在, 已插入!\n");
		else printf("\n找到!\n");

		if (display(tree) == 'c') break;
	}
	while (1)		// 删除测试
	{
		printf("请输入要删除的元素: ");
		scanf("%d", &data);
		if (!remove(NULL ,&tree, data))		// 删除
			printf("\n该元素不存在, 删除失败!\n");
		else printf("\n已删除!\n");

		if (display(tree) == 'c') break;
	}
	destory(tree);
	return 0;
}
