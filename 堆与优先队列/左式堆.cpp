
// 基于左式堆的优先队列
// 左式堆的左子树永运最深, 而右子树较浅,
// 所以右子树用来合并操作, 复杂度较小 O(log n)
// 若发现右子树的深度超过了左子树, 则执行旋转操作, 保持左边最深

#include <stdio.h>
#include <stdlib.h>

typedef int elemtype;
typedef int status;
#define OK 1
#define ERROR 0

// 左式堆趋于极不平衡, 用链式储存
typedef struct node
{
	elemtype data;
	node *lchild;
	node *rchild;
	int npl;
	// npl: 最短零路径长 (null path length),类似 AVL 树的平衡因子
}node, left_heap;


left_heap *merge(left_heap *a, left_heap *b);	// 合并驱动函数
left_heap *merge_s(left_heap *a, left_heap *b);	// 实际工作函数

// 合并 a, b 两个堆, 驱动函数, 把堆顶大的往小的合并
left_heap *merge(left_heap *a, left_heap *b)
{
	if (a == NULL) return b;
	else if (b == NULL) return a;
	else if (a->data < b->data)		// 堆顶小的在上
		return merge_s(a, b);
	else return merge_s(b, a);
}

// 把 b 合并到 a, a 的右子树较浅, 一直往右边合并
left_heap *merge_s(left_heap *a, left_heap *b)
{
	node *tmp;

	// a 结点小, 若 a 没有左子树了, 显然也没有右子树
	if (a->lchild == NULL) a->lchild = b;
	else 
	{
		a->rchild = merge(a->rchild, b);	// 还是往右合并
		if (a->rchild->npl > a->lchild->npl)	// 比较子树 npl
		{
			tmp = a->rchild;
			a->rchild = a->lchild;
			a->lchild = tmp;
		}	// 回溯时, 若发现右子树比较长, 则要左右旋转
		a->npl = a->rchild->npl + 1;	// 转完后, 调整双亲的 npl
	}
	return a;
}

// 插入可以看作是与单结点合并的特例
status insert(left_heap **h, elemtype data)
{
	left_heap *tmp;		// 让 h 与 tmp 进行合并

	tmp = (node *)malloc(sizeof(node));
	tmp->data = data;
	tmp->lchild = tmp->rchild = NULL;
	tmp->npl = 0;

	*h = merge(*h, tmp);	// 合并
	return OK;
}

status deletetop(left_heap **h)		// 删除堆顶
{
	node *del = *h;		// 记录待删除结点

	if (!(*h)) return ERROR;

	*h = merge((*h)->lchild, (*h)->rchild);	// 重新合并
	free(del);
	return OK;
}

status gettop(left_heap *h, elemtype *back)   // 获得堆顶
{
	if (!h) return ERROR;
	*back = h->data;
	return OK;
}

int check(left_heap *h)		// 检查堆的正确性
{
	int a, b;

	if (!h) return OK;
	if (!h->rchild) return OK;
	if (!h->lchild) return OK;

	// 若发现一个 npl 不符合左式堆, 就一直返回 ERROR
	if (h->lchild->npl < h->rchild->npl) return ERROR;
	a = check(h->lchild);
	b = check(h->rchild);
	if (a == ERROR || b == ERROR) return ERROR;
	else return OK;
}

void destory(left_heap *h)		// 销毁堆
{
	if (h)
	{
		if (h->lchild) destory(h->lchild);
		if (h->rchild) destory(h->rchild);
		free(h);
	}
}

void input(left_heap **h, int n)	// 输入模块
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
	left_heap *h = NULL;
	left_heap *h2 = NULL;
	int i, n;
	elemtype data;

	// 插入 n 个元素后, 全部出队, 就是优先队列了
	printf("建立堆1, 请插入 n 个元素: ");
	scanf("%d", &n);
	input(&h, n);
	if (check(h)) printf("\n堆正确!\n");
	else printf("\n堆错误!\n");

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
