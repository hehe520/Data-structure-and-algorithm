
// 红黑树的插入与删除算法, 查找和排序二叉树的一样
// 算法分析见 红黑树 - 算法分析.txt
// 主要函数:
// insert();			插入结点
// insert_fixup();		插入修复
// remove();			删除结点
// remove_fixup();		删除修复
// check();				红黑树结构检查


#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

typedef int status;
typedef int elemtype;
#define OK 1
#define ERROR 0
typedef enum {RED, BLACK} c_type;	// 颜色类型


// 红黑树结点结构
typedef struct node
{
	elemtype data;		// 数据域
	node *lchild;
	node *rchild;
	node *parent;
	c_type color;		// 结点颜色
}node, rb_tree;

node *nullnode = NULL;		// 哑结点, 避免检测空树


inline node *gradepa(node *tree)	// 求祖父
{
	return tree->parent->parent;
}

inline node *uncle(node *tree)		// 求叔叔
{
	node *g = gradepa(tree);

	if (tree->parent == g->lchild)
		return g->rchild;
	else return g->lchild;
}

inline node *brother(node *tree)	// 求兄弟
{
	if (tree == tree->parent->lchild)
		return tree->parent->rchild;
	else return tree->parent->lchild;
}

void left(node *tree)		// 单次左旋
{
	node *rc = tree->rchild;

	// rc 要升为新根
	if (tree->parent != nullnode) 
	{
		if (tree == tree->parent->lchild) 
			tree->parent->lchild = rc;
		else tree->parent->rchild = rc;
	}
	// 重接双亲
	if (rc->lchild != nullnode) 
		rc->lchild->parent = tree;
	rc->parent = tree->parent;
	tree->parent = rc;
	// 重接孩子
	tree->rchild = rc->lchild;
	rc->lchild = tree;
}

void right(node *tree)		// 单次右旋
{
	node *lc = tree->lchild;
	
	if (tree->parent != nullnode) 
	{
		if (tree == tree->parent->lchild) 
			tree->parent->lchild = lc;
		else tree->parent->rchild = lc;
	}
	if (lc->rchild != nullnode)
		lc->rchild->parent = tree;
	lc->parent = tree->parent;
	tree->parent = lc;
	tree->lchild = lc->rchild;
	lc->rchild = tree;
}

// 插入修复, 详细算法见 红黑树算法分析.txt
void insert_fixup(rb_tree *tree)
{
	node *g, *p;
	node *u = uncle(tree);

	if (tree->parent == nullnode)
		tree->color = BLACK;		// 新根, 直接涂黑
	else if (tree->parent->color == BLACK)
		return ;	// 父母是黑色的, 此时插入红色不用任何修复
	else if (u != nullnode
		&& u->color == RED)	// 对应情况 3
	{
		u->color = tree->parent->color = BLACK;
		g = gradepa(tree);
		g->color = RED;
		insert_fixup(tree->parent->parent);
		// 把祖父改成红色, 又可能会造成新的冲突, 重新递归修复
	}
	else	// 对应情况 4, 进行预处理, 全部化为一字型
	{
		g = gradepa(tree);
		p = tree->parent;		// 左右的之字形, 转为一字
		if (p == g->lchild && tree == p->rchild)
		{
			left(tree->parent);
			tree = tree->lchild;
		}
		else if (p == g->rchild && tree == p->lchild)
		{
			right(tree->parent);
			tree = tree->rchild;
		}
		// 下面对应情况 5
		tree->parent->color = BLACK;
		g = gradepa(tree);
		g->color = RED;
		if (tree == tree->parent->lchild 
			&& tree->parent == g->lchild)
			right(tree->parent->parent);
		else left(tree->parent->parent);
	}
}

// 递归插入部分
status insert_s(rb_tree **tree, elemtype data, node *f)
{
	node *n;

	if (*tree == nullnode)		// 没找了这个结点, 要插入
	{
		n = (node *)malloc(sizeof(node));
		n->color = RED;			// 新插入结点的颜色要是红的
		n->data = data;
		n->lchild = n->rchild = nullnode;
		n->parent = f;
		*tree = n;
		insert_fixup(n);	// 插入后, 可能会破坏性质, 进行插入修复
		// 这里一定要 n 的地址, 而不能用 tree
		// 因为这里的 tree 是上个结点孩子的地址, 会出错
		return OK;
	}
	else if ((*tree)->data == data) return ERROR;	// 已存在
	else if (data < (*tree)->data)
		return insert_s( &((*tree)->lchild), data, *tree);
	else return insert_s( &((*tree)->rchild), data, *tree);
}

// 先按照二叉树排序的方式插入, 之后再进行性质恢复
status insert(rb_tree **tree, elemtype data)
{
	status s;
	s = insert_s(tree, data, nullnode);
	while ((*tree)->parent != nullnode) 
		*tree = (*tree)->parent;
	return s;
}

// 删除 del 的双亲关系
void del_p(rb_tree *del, char r)
{
	if (del == del->parent->lchild)
	{
		if (r == 'L')
			del->parent->lchild = del->lchild;
		else del->parent->lchild = del->rchild;
	}
	else 
	{
		if (r == 'L')
			del->parent->rchild = del->lchild;
		else del->parent->rchild = del->rchild;
	}
}

// 删除修复, 详细算法见 红黑树算法分析.txt
void remove_fixup(rb_tree *tree, node *replace)
{
	node *s = brother(tree);

	if (tree->color == RED) return ;	// 删除红色的不用修复
	else 
	{	/// 此时 tree 是黑色的
		if (replace->color == RED)		// 替代点是红色的
			replace->color = BLACK;		// 改为黑色
		else
		{	// 此时 tree 和代替点都是黑色的
			if (tree->parent == nullnode) return ;
			else 
			{	// 对应 情况2, 把红色兄弟的红色移到自己的头上
				if (s->color == RED)
				{
					s->color = BLACK;
					tree->parent->color = RED;
					if (tree == tree->parent->lchild) 
						left(tree->parent);
					else right(tree->parent);
				}
				// 下面对应 情况3, 全部都是黑的
				s = brother(tree);
				if (tree->parent->color == BLACK 
					&& s->color == BLACK 
					&& s->lchild->color == BLACK 
					&& s->rchild->color == BLACK)
				{
					s->color = RED;		// 给兄弟加个红色
					remove_fixup(tree->parent, tree);
				}
				else	// 情况 4, 把双亲的红色移到兄弟中
				{
					if (tree->parent->color == RED 
						&& s->color == BLACK 
						&& s->lchild->color == BLACK 
						&& s->rchild->color == BLACK)
					{
						tree->parent->color = BLACK;
						s->color = RED;
					}
					else	// 情况5, 兄弟的孩子中有红色 
					{		// 要转成一字型
						if (s->color == BLACK)
						{
							if (tree == tree->parent->lchild
								&& s->lchild->color == RED 
								&& s->rchild->color == BLACK)
							{
								s->color = RED;
								s->lchild->color = BLACK;
								right(s);
							}
							else if (tree == tree->parent->rchild
								&& s->lchild->color == BLACK
								&& s->rchild->color == RED)
							{
								s->color = RED;
								s->rchild->color = BLACK;
								left(s);
							}
						}
						// 情况6, 向 tree 的头上增加一个黑色
						s = brother(tree);
						s->color = tree->parent->color;
						tree->parent->color = BLACK;
						if (tree == tree->parent->lchild)
						{
							s->rchild->color = BLACK;
							left(tree->parent);
						}
						else 
						{
							s->lchild->color = BLACK;
							right(tree->parent);
						} // case 6
					} // case 5
				} // case 4
			} // case 3
		} // case 2
	} // csae 1
}

// 先按照二叉排序树的方式找到该结点
// 把通过该结点的简单路径增加一个黑色结点, 再释放掉它
status remove_s(rb_tree **tree, elemtype data)
{
	node *pre;
	node *del;

	if ((*tree) == nullnode) return ERROR;	// 没有这个结点
	else if ((*tree)->data == data)		// 找到了
	{
		// 此时要删除 *tree, 采用前驱替换法删除
		// 真正删除的不是 *tree 的空间, 而是它的前驱
		// 删去一个结点前, 调整双亲关系, 调整孩子关系
		del = *tree;
		switch ((del->lchild != nullnode) + 
			2 * (del->rchild != nullnode))
		{
		case 0 :					// 左右孩子皆空
			remove_fixup(del, del->lchild);		// 删除修复
			if (del->parent == nullnode) *tree = nullnode;
			else del_p(del, 'L');   break;
		case 1 :					// 只有左孩子
			remove_fixup(del, del->lchild);
			del->lchild->parent = del->parent;
			if (del->parent == nullnode) 
				*tree = del->lchild;
			else del_p(del, 'L');   break;
		case 2 :					// 只有右孩子
			remove_fixup(del, del->rchild);
			del->rchild->parent = del->parent;
			if (del->parent == nullnode)
				*tree = del->rchild;
			else del_p(del, 'R');   break;
		case 3 :					// 两个都有
			pre = del->lchild;
			while (pre->rchild != nullnode) pre = pre->rchild;
			del->data = pre->data;		// 前驱替换
			remove_fixup(pre, pre->lchild);
			pre->lchild->parent = pre->parent;
			del = pre;
			del_p(pre, 'L');   break;
		}
		free(del);
		return OK;
	}
	else if (data < (*tree)->data)
		return remove_s( &((*tree)->lchild), data);
	else return remove_s( &((*tree)->rchild), data);
}

status remove(rb_tree **tree, elemtype data)
{
	status s;
	node *cur = *tree;

	s = remove_s(tree, data);
	if (*tree != nullnode 
		&& (*tree)->parent != nullnode) 
	{
		cur = *tree;
		while (cur->parent != nullnode)
			cur = cur->parent;
		*tree = cur;
	}
	// 由于指针问题, 旋转的时候根会被替换, 这里重新回到根
	return s;
}

// 检查所以路径的黑色结点是不是一样多
int check_path(rb_tree *tree)
{
	int left, right;

	if (tree == nullnode) return 1;		// 叶子也是黑色的

	left = check_path(tree->lchild);
	right = check_path(tree->rchild);

	if (left == 0 || right == 0) return ERROR;
	if (left != right) return ERROR;	// 黑色结点数不一样了

	if (tree->color == BLACK) return left + 1;	// 黑色的才加一个
	else return left;
}

status check_child(rb_tree *tree)	// 检查有没有红色相连的结点
{
	if (tree == nullnode) return OK;
	if (tree->color == RED)
	{
		// 不用检测树的孩子是不是空的, 因为有哑结点的存在
		// 而哑结点的颜色是黑色
		if (tree->lchild->color == RED) return ERROR;
		if (tree->rchild->color == RED) return ERROR;
	}
	if (!check_child(tree->lchild)) return ERROR;
	if (!check_child(tree->rchild)) return ERROR;
	return OK;
}

status check(rb_tree *tree)		// 红黑树性质检测
{
	if (tree == nullnode) return OK;		// 空树也是红黑树
	if (tree->color == RED) return ERROR;	// 根是红的
	if (!check_child(tree)) return ERROR;	// 有红色相连的结点
	if (!check_path(tree)) return ERROR;	// 所有路径黑色结点不一样多
	return OK;
}

node *init()		// 初始化
{
	if (!nullnode)
	{
		nullnode = (node *)malloc(sizeof(node));
		nullnode->color = BLACK;		// 叶子结点要是黑色的
		nullnode->lchild = nullnode->rchild = nullnode;
		nullnode->parent = nullnode;
		nullnode->data = -1;
	}
	return nullnode;
}

void destory(rb_tree *tree)		// 销毁
{
	if (tree != nullnode)
	{
		destory(tree->lchild);
		destory(tree->rchild);
		free(tree);
	}
}

void mid_tra(rb_tree *tree)		// 先序遍历
{
	if (tree != nullnode)
	{
		mid_tra(tree->lchild);
		printf("%d, ", tree->data);
		mid_tra(tree->rchild);
	}
}

void first(rb_tree *tree)		// 中序遍历
{
	if (tree != nullnode)
	{
		printf("\ndata = %d, parent = %d,  color = %s", 
			tree->data, tree->parent->data, 
			tree->color==RED ? "Red" : "Black");
		first(tree->lchild);
		first(tree->rchild);
	}
}

int depth(rb_tree *tree)	// 求深度
{
	int right, left;

	if (tree == nullnode) return 0;

	left = depth(tree->lchild);
	right = depth(tree->rchild);
	return (left > right ? left : right) + 1;
}

void ASL_s(rb_tree *tree, int *asl, int *cnt, int flood)
{
	if (tree != nullnode) 
	{
		*asl += flood;
		(*cnt)++;
		ASL_s(tree->lchild, asl, cnt, flood+1);
		ASL_s(tree->rchild, asl, cnt, flood+1);
	}
}

float ASL(rb_tree *tree)		// 求 ASL
{
	int asl = 0;
	int cnt = 0;
	ASL_s(tree, &asl, &cnt, 1);
	if (cnt == 0) return 0.0;
	else return (float)asl / (float)cnt;
}

char display(rb_tree *tree)	// 界面显示模块
{
	char c;

	printf("\n先序遍历: ");   first(tree);
	printf("\n\n中序遍历: ");   mid_tra(tree);
	printf("\n\n深度: %d\t\t", depth(tree));
	printf("平均 ASL = %0.3f\t", ASL(tree));
	if (check(tree)) printf("红黑树正确");
	else printf("结构错误");
	
	printf("\n\n按 c 键结束, 按回车继续...\n");
	getchar();   c = getch();
	system("cls");

	return c;
}

int main(void)
{
	rb_tree *tree;
	elemtype data;

	tree = init();
	while (1)		// 插入测试
	{
		printf("请输入要插入的元素: ");
		scanf("%d", &data);

		if (insert(&tree, data))	// 插入
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
	if (nullnode) free(nullnode);
	return 0;
}
