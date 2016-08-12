
// 基于斜堆的优先队列
// 斜堆与左式堆的区别在于, 斜堆不保存 npl 信息
// 斜堆一直往右子树合并, 这样会造成右子树过程
// 为了解决这个问题, 斜堆在递归合并的时候, 对于每一个临时的结点
// 进行旋转, 即交换他们的左右子树, 这类似棉花糖的旋转

#include <stdio.h>
#include <stdlib.h>

typedef int elemtype;
typedef int status;
#define OK 1
#define ERROR 0

// 斜堆链式储存
typedef struct node
{
	elemtype data;
	node *lchild;
	node *rchild;
}node, xie_heap;


xie_heap *merge(xie_heap *a, xie_heap *b);		// 合并驱动函数
xie_heap *merge_s(xie_heap *a, xie_heap *b);	// 实际工作函数

// 合并 a, b 两个堆, 驱动函数, 把堆顶大的往小的合并
xie_heap *merge(xie_heap *a, xie_heap *b)
{
	if (a == NULL) return b;
	else if (b == NULL) return a;
	else if (a->data < b->data)
		return merge_s(a, b);
	else return merge_s(b, a);
}

// 把 b 合并到 a, 此时 a 的堆顶小
xie_heap *merge_s(xie_heap *a, xie_heap *b)
{
	node *tmp;

	if (a->lchild == NULL) a->lchild = b;
	else 
	{
		a->rchild = merge(a->rchild, b);	// 合并右孩子
		tmp = a->lchild;			// 回溯时, 旋转左右孩子
		a->lchild = a->rchild;
		a->rchild = tmp;
	}
	return a;
}

// 插入可以看作是与单结点合并的特例
status insert(xie_heap **h, elemtype data)
{
	node *n;

	n = (node *)malloc(sizeof(node));
	n->lchild = n->rchild = NULL;
	n->data = data;

	*h = merge(*h, n);
	return OK;
}

status deletetop(xie_heap **h)		// 删除堆顶
{
	node *del = *h;		// 堆顶待删除

	*h = merge((*h)->lchild, (*h)->rchild);
	free(del);
	return OK;
}

status gettop(xie_heap *h, elemtype *back)   // 获得堆顶
{
	if (!h) return ERROR;
	*back = h->data;
	return OK;
}

void destory(xie_heap *h)		// 销毁堆
{
	if (h)
	{
		if (h->lchild) destory(h->lchild);
		if (h->rchild) destory(h->rchild);
		free(h);
	}
}

int lenth(xie_heap *h)		// 求深度
{
	int left, right;

	if (!h) return 0;
	left = lenth(h->lchild);
	right = lenth(h->rchild);

	return (left > right ? left : right) + 1;
}

void input(xie_heap **h, int n)	// 输入模块
{
	int data;

	while (n--)
	{
		scanf("%d", &data);
//		data = rand() % 1000;	// (测试用) 不想手动输入, 就自动吧
		insert(h, data);		// 插入测试
	}
}

int main(void)
{
	xie_heap *h = NULL;
	xie_heap *h2 = NULL;
	int i, n;
	elemtype data;

	// 插入 n 个元素后, 全部出队, 就是优先队列了
	printf("建立堆1, 请插入 n 个元素: ");
	scanf("%d", &n);
	input(&h, n);
	printf("\n堆深度: %d\n", lenth(h));

	printf("\n弹出 n 个元素: ");
	scanf("%d", &n);
	for (i=0; i<n; i++)
	{
		if (gettop(h, &data))
		{
			printf("%d, ", data);
			deletetop(&h);		// 同时删除堆顶
		}
		else break;
	}
	printf("\n\n建立堆2, 请插入 n 个元素: ");
	scanf("%d", &n);
	input(&h2, n);

	printf("\n合并堆1 堆2 ......\n");
	h = merge(h, h2);

	printf("弹出堆中全部的元素: ");
	while (gettop(h, &data))
	{
		printf("%d, ", data);
		deletetop(&h);
	}
	printf("\n\n");
	destory(h);
	return 0;
}
