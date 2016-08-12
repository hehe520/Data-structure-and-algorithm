
// 伸展树的基本操作
// search()		查找
// insert()		插入
// remove()		删除
// splay()		伸展

// 伸展树的核心操作, splay(tree, x) 伸展 x 结点, 
// 也就是把 x 结点提到根的位置, 同时不破坏二叉查找树的性质
// 如果没有 x 结点, 那就伸展 x 的前驱或者后继结点, 也可以什么都不伸展

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#define OK 1
#define ERROR 0
typedef int status;
typedef int elemtype;

// 伸展树用二叉链表储存
typedef struct node
{
	elemtype data;
	node *lchild;
	node *rchild;
}node, splay_tree;

static node *nullnode = NULL;	// 哑结点, 避免检测空树

node *init(void)	// 初始化伸展树, 分配一个哑结点
{
	if (!nullnode)		// 用 nullnode 代替 NULL, 简化程序
	{
		nullnode = (node *)malloc(sizeof(node));
		nullnode->lchild = nullnode->rchild = nullnode;
	}
	return nullnode;
}

// 伸展 data 结点, 如果没有就伸展 data 的前驱
// 也就是把 data 提到根处
// 自下而上的伸展方式不便于编程, 这里采用自上而下的方法
node *splay(splay_tree **tree, elemtype data)
{
	node tmp;
	node *left_min, *right_max;		// 临时的左右子树, BST性质, 左边小, 右边大
	node *cur;

	tmp.lchild = tmp.rchild = nullnode;
	left_min = right_max = &tmp;
	nullnode->data = data;		// data 暂存在哑结点中

	if (*tree == nullnode) return *tree;		// 空树
	
	while ((*tree)->data != data)		// 开始伸展 data
	{
		if (data < (*tree)->data)		// 在左子树中找
		{
			// 简化的之字型
			// 这里 tree->left 一定不是 NULL, 因为有哑结点 nullnode
			// 这就是设置哑结点的目的, 减少特殊情况的判断, 简化程序
			if (data < (*tree)->lchild->data)
			{
				// 左倾一字型, 向右旋转, 和 AVL 树的旋转一样
				cur = (*tree)->lchild;
				(*tree)->lchild = cur->rchild;
				cur->rchild = *tree;
				*tree = cur;
			}
			if ((*tree)->lchild == nullnode) break;
			right_max->lchild = *tree;		// 接入到临时右树中
			right_max = right_max->lchild;		// 最左边的是最小的, 所以接在左边不破坏 BST 性质
			*tree = (*tree)->lchild;
		}
		else if (data > (*tree)->data)	// 在右子树中找
		{
			if (data > (*tree)->rchild->data)
			{
				// 右倾一字型, 向左旋转
				cur = (*tree)->rchild;
				(*tree)->rchild = cur->lchild;
				cur->lchild = *tree;
				*tree = cur;
			}
			if ((*tree)->rchild == nullnode) break;
			left_min->rchild = *tree;		// 最右边是最大的
			left_min = left_min->rchild;
			*tree = (*tree)->rchild;
		}
	}
	// 把左右两个临时根接回来
	left_min->rchild = (*tree)->lchild;
	right_max->lchild = (*tree)->rchild;
	(*tree)->lchild = tmp.rchild;
	(*tree)->rchild = tmp.lchild;

	return *tree;
}

// 在树中插入一个 data
// 把与 data 最相近的结点(前驱)伸展到根处, 
// 在把 data 接入到树中, 如果有了就不重复接入了, 其实也可以重复接入
status insert(splay_tree **tree, elemtype data)
{
	node *new_node;		// 新结点, 待插入

	new_node = (node *)malloc(sizeof(node));
	new_node->lchild = new_node->rchild = nullnode;
	new_node->data = data;

	*tree = splay(tree, data);		// 伸展
	if (*tree == nullnode)  *tree = new_node;		// 空树
	else if (data == (*tree)->data) {free(new_node);  return ERROR;}
	else if (data < (*tree)->data)		// 看看把 new_node 接在哪边
	{
		new_node->lchild = (*tree)->lchild;
		new_node->rchild = *tree;
		(*tree)->lchild = nullnode;
	}
	else 
	{
		new_node->lchild = *tree;
		new_node->rchild = (*tree)->rchild;
		(*tree)->rchild = nullnode;
	}
	*tree = new_node;
	return OK;
}

// 查找一个元素, 同时伸展它
// 如果找不到就伸展它的前驱
status search(splay_tree **tree, elemtype data)
{
	*tree = splay(tree, data);
	if (data == (*tree)->data) return OK;
	else return ERROR;
}

// 删除一个元素
// 尝试将该元素伸展到根, 如果有, 就释放它
status remove(splay_tree **tree, elemtype data)
{
	node *del;

	if (*tree == nullnode) return ERROR;

	*tree = splay(tree, data);
	if ((*tree)->data == data)		// 找到
	{
		del = *tree;
		if ((*tree)->lchild == nullnode)
		{
			if ((*tree)->rchild == nullnode)
				*tree = nullnode;
			else *tree = (*tree)->rchild;
		}
		else if ((*tree)->rchild == nullnode)
			*tree = (*tree)->lchild;
		else		// 左右都不空, 在左子树中伸展一个最大的到根
		{			// 根据 BST 性质, 最大的是根, 就没有右子树了
			*tree = splay(&((*tree)->lchild), data);
			(*tree)->rchild = del->rchild;
		}
		free(del);
		return OK;
	}
	return ERROR;
}

void destory(splay_tree *tree)		// 销毁
{
	if (tree != nullnode)
	{
		destory(tree->lchild);
		destory(tree->rchild);
		free(tree);
	}
}

void first_tra(splay_tree *tree)	// 先序遍历
{
	if (tree != nullnode)
	{
		printf("%d, ", tree->data);
		first_tra(tree->lchild);
		first_tra(tree->rchild);
	}
}

void mid_tra(splay_tree *tree)		// 中序遍历
{
	if (tree != nullnode)
	{
		mid_tra(tree->lchild);
		printf("%d, ", tree->data);
		mid_tra(tree->rchild);
	}
}

char display(splay_tree *tree)	// 界面显示模块
{
	char c;

	printf("\n先序遍历: ");   first_tra(tree);
	printf("\n\n中序遍历: ");   mid_tra(tree);
	
	printf("\n\n按 c 键结束, 按回车继续...\n");
	getchar();   c = getch();
	system("cls");

	return c;
}

int main(void)
{
	splay_tree *tree;
	int data;

	tree = init();		// 初始化
	while (1)		// 插入测试
	{
		printf("请输入要插入的元素: ");
		scanf("%d", &data);
		if (insert(&tree, data))		// 插入
			printf("\n该元素不存在, 已插入!\n");
		else printf("\n%d 已存在!\n", data);

		if (display(tree) == 'c') break;
	}
	while (1)		// 查找测试
	{
		printf("请输入要查找的元素: ");
		scanf("%d", &data);
		if (search(&tree, data))
			printf("\n找到!\n");
		else printf("\n未找到! %d\n", data);

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
