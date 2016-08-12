
// 平衡二叉树的相关操作 (AVL 树)
// 主要操作, 插入 insertAVL(), 删除 deleteAVL()
// 人类文字已经不能描述该算法了, 看 235 页的四幅图

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

typedef int elemtype;
typedef int status;
#define OK 1
#define ERROR 0

// LH, EH, RH, 分别表示左树高了, 平衡, 右树高了
#define LH +1
#define EH  0
#define RH -1

// 平衡二叉树储存结构
typedef struct bnode
{
	short int bf;			// 平衡因子
	elemtype data;
	bnode *lchild;
	bnode *rchild;
}AVLtree, bnode;

// -----------------平衡二叉树的旋转算法-------------------
// 进行一次左旋, 该算法可以看着图示来写
void left(AVLtree **tree)
{
	bnode *rc = (*tree)->rchild;
	if (rc)
	{
		(*tree)->rchild = rc->lchild;
		rc->lchild = *tree;
		*tree = rc;		// 右孩子成为新根
	}
}

void right(AVLtree **tree)		// 进行一次右旋
{
	bnode *lc = (*tree)->lchild;
	if (lc)
	{
		(*tree)->lchild = lc->rchild;
		lc->rchild = *tree;
		*tree = lc;		// 左孩子成为新根
	}
}

// 右平衡的通俗解释: 把二叉树往右边扯
void rightbalence(AVLtree **tree)	// 进行右平衡
{
	bnode *lc = (*tree)->lchild;

	// 算法见 235 页图 (a)
	if (lc->bf == LH)	// 通过平衡因子, 看看插入到了哪边
	{
		(*tree)->bf = lc->bf = EH;
		right(tree);
	}
	else	// lc->bf == RH, 插入到了 lc 右边
	{
		// 先调整旋转后的平衡因子, 算法见图 (b)
		switch (lc->rchild->bf)
		{
		case LH : 
			lc->bf = EH;   (*tree)->bf = RH;   break;
		case EH : 
			lc->bf = (*tree)->bf = EH;   break;
			// 这种情况是 lc->rchild 是叶子结点的情况
			// 调整完后, 全部平衡, 所以是 EH
		case RH : 
			lc->bf = LH;   (*tree)->bf = EH;   break;
		}
		lc->rchild->bf = EH;
		// 先调整好因子, 在调整结点
		right( &((*tree)->lchild) );
		left(tree);
	}
}

void leftbalence(AVLtree **tree)	// 进行左平衡
{
	bnode *rc = (*tree)->rchild;

	if (rc->bf == RH)		// 算法见图 (c)
	{
		rc->bf = (*tree)->bf = EH;
		left(tree);
	}
	else 
	{
		switch (rc->lchild->bf)	// 算法见图 (d)
		{
		case LH : 
			(*tree)->bf = EH;   rc->bf = RH;   break;
		case EH : 
			(*tree)->bf = rc->bf = EH;   break;
		case RH : 
			(*tree)->bf = LH;   rc->bf = EH;   break;
		}
		rc->lchild->bf = EH;
		right( &((*tree)->rchild) );
		left(tree);
	}
}

// ---------------------平衡二叉树的插入算法-----------------------
// taller 来表示树是否变高了, 若变高了, 要调整平衡因子
// 若调整平衡因子后, 发现绝对值超过了 2, 要进行旋转操作
status insertAVL(AVLtree **tree, elemtype data, short int *taller)
{
	if (!(*tree))		// 没有这个结点就插入
	{
		*tree = (bnode *)malloc(sizeof(bnode));
		(*tree)->bf = EH;
		(*tree)->lchild = (*tree)->rchild = NULL;
		(*tree)->data = data;
		*taller = 1;	// 告诉上一层函数, 高度发生的变化, 要进行因子调整
	}
	else if ((*tree)->data == data)	// 存在了
	{
		*taller = 0;
		return ERROR;
	}
	else if (data < (*tree)->data)
	{
		if (!insertAVL( &((*tree)->lchild), data, taller))
			return ERROR;

		// 此时已插入左子树, 回溯时, 看看高度变化没
		if (*taller)		// 如果高度变化, 要调整子根的平衡因子
		{
			switch ((*tree)->bf)
			{
			case LH :	// 本来左边高的, 现在左边又多一个, 要进行右旋保持平衡
				rightbalence(tree);
				*taller = 0;
				break;
			case EH :			// 本来平衡的, 现在左边多了一个
				(*tree)->bf = LH;
				*taller = 1;		// 高度增加, 造成新的不平衡
				break;
			case RH :			// 本来右边高的, 现在左边又多一个, 所以平衡
				(*tree)->bf = EH;
				*taller = 0;		// 高度没变化
				break;
			}
		}
	}
	else	// 插在右子树上
	{
		if (!insertAVL( &((*tree)->rchild), data, taller))
			return ERROR;
		if(*taller)
		{
			// 插入到了右子树上, 且高度变化了, 调整因子同理
			switch ((*tree)->bf)
			{
			case LH : 
				(*tree)->bf = EH;   *taller = 0;   break;
			case EH : 
				(*tree)->bf = RH;   *taller = 1;   break;
			case RH : 
				leftbalence(tree);  *taller = 0;   break;
				// 右边更高了, 进行左旋
			}
		}
	}
	return OK;
}

// ------------------平衡二叉树删除算法-------------------
// 先找到该结点, 进行删除, 删除后, 再进行调整至平衡
status delete_s(bnode *p, bnode **tree)
{
	bnode *del = *tree;		// 待删除结点
	bnode *pre = NULL;		// del 前驱
	bnode *fpre = NULL;		// del 前驱的双亲

	if (!del) return ERROR;
	if (!del->lchild)			// 左孩子空
	{
		if (!p) *tree = del->rchild;
		else if (p->lchild == del) p->lchild = del->rchild;
		else p->rchild = del->rchild;
	}
	else if (!(*tree)->rchild)		// 右孩子空
	{
		if (!p) *tree = del->lchild;
		else if (p->lchild == del) p->lchild = del->lchild;
		else p->rchild = del->lchild;
		
	}
	else	// 左右都不空
	{
		pre = del->lchild;
		while (pre->rchild) 
		{
			fpre = pre;
			pre = pre->rchild;
		}
		del->data = pre->data;		// 前驱替换
		if (!fpre) del->lchild = pre->lchild;
		else fpre->rchild = pre->lchild;
		del = pre;		// 删除前驱
	}
	free(del);
	return OK;
}

// 找到并删除, p 是双亲结点, tree 树根, data 待删除
status find_and_del(AVLtree *p, AVLtree **tree, elemtype data)
{
	if (!(*tree)) return ERROR;
	else if ((*tree)->data == data) return delete_s(p, tree);
	else if (data < (*tree)->data)
		return find_and_del(*tree, &( (*tree)->lchild ), data);
	else return find_and_del(*tree, &( (*tree)->rchild ), data);
}

// 将一颗任意树, 调整为平衡二叉树
int adjust(AVLtree **tree)
{
	int left, right;	// 记录左右深度

	if (!(*tree)) return 0;
	left = adjust( &((*tree)->lchild) );
	right = adjust( &((*tree)->rchild) );

	// 回溯的时候, 不断调整
	(*tree)->bf = left - right;
	if ((*tree)->bf > 1)		// 左边高了
		rightbalence(tree);
	else if ((*tree)->bf < -1)	// 右边高了
		leftbalence(tree);

	// 返回深度
	return (left > right ? left : right) + 1;
}

// 删除结点的主函数
status deleteAVL(AVLtree *p, AVLtree **tree, elemtype data)
{
	if (find_and_del(p, tree, data))
	{
		adjust(tree);		// 调整子树到平衡
		return OK;
	}
	return ERROR;
}

// 递归求 ASL, num 结点个数, len 总长度, flood 递归层数
void ASL_s(AVLtree *tree, int *num, double *len, int flood)
{
	if (!tree) return ;

	(*num)++;   (*len) += flood;
	// 累计完当前层的, 继续向下递归
	ASL_s(tree->lchild, num, len, flood + 1);
	ASL_s(tree->rchild, num, len, flood + 1);
}

double ASL(AVLtree *tree)		// 计算平均查找长度 ASL
{
	int num = 0;
	double len = 0.0;

	ASL_s(tree, &num, &len, 1);
	if (num == 0) return 0;
	else return len / num;
}

int balence(AVLtree *tree)		// 判断是否平衡
{
	int a, b;

	if (!tree) return 0;

	a = balence(tree->lchild);
	b = balence(tree->rchild);

	// 发现一个不平衡就返回 -1, 否则返回深度
	if (a == -1 || b == -1 || abs(a - b) > 1) return -1;
	else return (a > b ? a : b) + 1;
}

int depth(AVLtree *tree)	// 求深度
{
	int a, b;

	if (!tree) return 0;
	a = depth(tree->lchild);
	b = depth(tree->rchild);
	return (a > b ? a : b) + 1;
}

void mid_tra(AVLtree *tree)	// 中序遍历
{
	if (tree)
	{
		mid_tra(tree->lchild);
		printf("%d, ", tree->data);
		mid_tra(tree->rchild);
	}
}

void first(AVLtree *tree)	// 先序遍历
{
	if (tree)
	{
		printf("%d, ", tree->data);
		first(tree->lchild);
		first(tree->rchild);
	}
}

void destory(AVLtree *tree)	// 销毁树
{
	if (tree)
	{
		destory(tree->lchild);
		destory(tree->rchild);
		free(tree);
	}
}

char display(AVLtree *tree)	// 界面显示模块
{
	char c;

	printf("\n先序遍历: ");
	first(tree);
	printf("\n中序遍历: ");
	mid_tra(tree);
	printf("\n\n平均查找长度: %.4lf         ", ASL(tree));
	printf("深度: %d     ", depth(tree));
	if (balence(tree) == -1) printf("树不平衡!\n");
	else printf("平衡!\n");
	
	printf("\n按 c 键结束, 按回车继续...\n");
	getchar();    c = getch();
	system("cls");
	return c;
}

int main(void)
{
	AVLtree *tree = NULL;
	short int taller;
	int data;
	short int times = 2;

	while (times--)
	{
		while (1)		// 插入测试
		{
			printf("请输入要查找的元素: ");
			scanf("%d", &data);
			if (insertAVL(&tree, data, &taller))	// 插入
				printf("\n该元素不存在, 已插入!\n");
			else printf("\n找到!\n");
			
			if (display(tree) == 'c') break;
		}
		while (1)		// 删除测试
		{
			printf("请输入要删除的元素: ");
			scanf("%d", &data);
			if (deleteAVL(NULL, &tree, data))	// 删除
				printf("\n成功删除!\n");
			else printf("\n该元素不存在!\n");
			
			if (display(tree) == 'c') break;
		}
	}
	// 再次插入...
	destory(tree);
	return 0;
}
