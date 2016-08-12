#include <stdio.h>
#include <stdlib.h>

// 二叉树排序, 稍作修改, 允许插入相同的元素

typedef struct node		// 二叉树结构
{
	int data;
	node *lc;
	node *rc;
}btree, node;


// 在树上查找一个插入位置, 直到失败返回双亲
// 允许有相同的结点, 并且规定, 左子树 <= 根 < 右子树
void search(btree *tree, node *f, int data, node **p)
{
	if (!tree) *p = f;
	else if (data <= tree->data)
		search(tree->lc, tree, data, p);
	else search(tree->rc, tree, data, p);
}

// 插入一个的结点, 允许插入相同的结点
void insert(btree **tree, int data)
{
	node *p = NULL;
	node *n;

	search(*tree, NULL, data, &p);
	n = (node *)malloc(sizeof(node));
	n->lc = n->rc = NULL;
	n->data = data;
	if (!p) *tree = n;
	else if (data <= p->data) p->lc = n;
	else p->rc = n;
}

// 中序遍历, 并且将结果保存到, data 中
void mid_tra(btree *tree, int *data, int *pos)
{
	if (tree)
	{
		mid_tra(tree->lc, data, pos);
		data[ (*pos)++ ] = tree->data;
		mid_tra(tree->rc, data, pos);
	}
}

void destory(btree *tree)	// 销毁树
{
	if (tree)
	{
		destory(tree->lc);
		destory(tree->rc);
		free(tree);
	}
}

void btree_sort(int *data, int n)	// 二叉树排序
{
	btree *tree = NULL;
	int i, pos = 0;		// pos 指示 data 数据位置

	for (i=0; i<n; i++) insert(&tree, data[i]);
	mid_tra(tree, data, &pos);	
	destory(tree);
}

int main(void)
{
	int i, n;
	int data[100];

	printf("请输入 n, 及 n 个元素: ");
	scanf("%d", &n);
	for (i=0; i<n; i++) scanf("%d", data + i);

	btree_sort(data, n);
	printf("排序后: ");
	for (i=0; i<n; i++) printf("%d, ", data[i]);
	printf("\n\n");
	return 0;
}
