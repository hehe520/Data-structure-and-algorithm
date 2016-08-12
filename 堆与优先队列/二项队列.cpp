
// 基于二项树的优先队列

#include <stdio.h>
#include <stdlib.h>

typedef int elemtype;
typedef int status;
#define OK 1
#define ERROR 0
#define INF 0x5FFFFFFF
#define MAX 30					// 树根数量
#define capacity 1073741824		// 结点数量


// 孩子兄弟储存二项树
typedef struct node
{
	elemtype data;
	node *fchild;		// 首孩子
	node *nbrother;		// 兄弟
}node;

// 二项树
typedef struct bqueue
{
	node *base[MAX];		// 二项树根
	int cap;		// 结点数量
}bqueue;


bqueue *init(void)		// 创建一个二项队列
{
	bqueue *q;
	int i;

	q = (bqueue *)malloc(sizeof(bqueue));
	for (i=0; i<MAX; i++)
		q->base[i] = NULL;
	q->cap = 0;
	return q;
}

node *new_node(elemtype data)	// 新建结点
{
	node *n;

	n = (node *)malloc(sizeof(node));
	n->data = data;
	n->fchild = n->nbrother = NULL;
	return n;
}

void del(node *n)		// 销毁一个子树
{
	if (n)
	{
		del(n->fchild);
		del(n->nbrother);
		free(n);
	}
}

void destory(bqueue *q)		// 销毁二项队列
{
	int i;

	if (!q) return ;
	for (i=0; i<MAX; i++)
		if (q->base[i]) del(q->base[i]);
	free(q);
}

// 合并两个相同的子树, 保存到 n1
node *combine(node *n1, node *n2)
{
	if (n1->data < n2->data)
	{
		n2->nbrother = n1->fchild;
		n1->fchild = n2;
		return n1;
	}
	else 
	{
		n1->nbrother = n2->fchild;
		n2->fchild = n1;
		return n2;
	}
}

// 合并 h1, h2, 同时销毁 h2
bqueue *merge(bqueue **h1, bqueue **h2)
{
	node *t1, *t2, *carry;
	int i, j;

	if (*h1 == NULL) return *h2;
	if (*h2 == NULL) return *h1;
	// 超容量了
	if ((*h1)->cap + (*h2)->cap > capacity) return NULL;
	carry = NULL;
	(*h1)->cap += (*h2)->cap;
	for (i=0, j=1; j<=(*h1)->cap; i++, j*=2)	// j 控制大小
	{
		t1 = (*h1)->base[i];   t2 = (*h2)->base[i];
		// 421 码判断 t1, t2, carry 状态
		// 任何一个数值经过 !! 运算变成 0,1
		switch (!!t1 + !!t2 * 2 + !!carry * 4)
		{
		case 0 : break;		// 没有树
		case 1 : break;		// 只有 t1
		case 2 :			// 只有 t2
			(*h1)->base[i] = t2;
			(*h2)->base[i] = NULL;  break;
		case 3 :
			carry = combine(t1, t2);	// t1, t2
			(*h1)->base[i] = NULL;
			(*h2)->base[i] = NULL;  break;
		case 4 :	// 只有 carry
			(*h1)->base[i] = carry;
			carry = NULL;  break;		// 记得要及时清空carry
		case 5 :	// t1, carry
			carry = combine(carry, t1);
			(*h1)->base[i] = NULL;  break;
		case 6 :	// t2, carry
			carry = combine(carry, t2);
			(*h2)->base[i] = NULL;  break;
		case 7 :	// t1, t2, carry
			(*h1)->base[i] = carry;
			carry = combine(t1, t2);
			(*h2)->base[i] = NULL;  break;
		}
	}
	return *h1;
}

status insert(bqueue **q, elemtype data)		// 向 q 中插入一个元素
{
	bqueue *tmp;

	tmp = init();
	tmp->base[0] = new_node(data);
	tmp->cap = 1;

	*q = merge(q, &tmp);
	destory(tmp);
	if (*q == NULL) return ERROR;
	else return OK;
}

// 返回最小值, 同时删去它
elemtype deletetop(bqueue **h)
{
	elemtype mindata = INF;
	int minpos = -1;
	int i, j;
	node *tree, *del;
	bqueue *tmp;


	// 找到最小元 mindata, 及位置 minpos
	for (i=0, j=1; *h && j<=(*h)->cap; i++, j*=2)
	{
		if ((*h)->base[i] && (*h)->base[i]->data < mindata)
		{
			mindata = (*h)->base[i]->data;
			minpos = i;
		}
	}
	if (minpos == -1) return INF;
	// 将第 minpos 的子树拿出来, 分散到临时队列中, 再合并到 h 中
	del = (*h)->base[minpos];
	(*h)->base[minpos] = NULL;
	tree = del->fchild;
	free(del);
	tmp = init();
	tmp->cap = 1 << minpos;
	for (i=minpos-1; i>=0 && tree; i--)
	{
		tmp->base[i] = tree;
		tree = tree->nbrother;
		tmp->base[i]->nbrother = NULL;
	}
	(*h)->cap -= tmp->cap;
	*h = merge(h, &tmp);
	return mindata;
}

void input(bqueue **q, int n)
{
	elemtype data;

	while (n--)
	{
		scanf("%d", &data);
//		data = rand() % 1000;	// (测试用) 不想手动输入, 就自动吧
		insert(q, data);		// 插入测试
	}
}

int main(void)
{
	bqueue *h = NULL;
	bqueue *h2 = NULL;
	int i, n;
	elemtype data;

	// 插入 n 个元素后, 全部出队, 就是优先队列了
	h = init();
	printf("建立优先队列1, 请插入 n 个元素: ");
	scanf("%d", &n);
	input(&h, n);

	printf("\n弹出 n 个元素: ");
	scanf("%d", &n);
	for (i=0; i<n; i++)
	{
		data = deletetop(&h);
		if (data != INF)
			printf("%d, ", data);
	}
	h2 = init();
	printf("\n\n建立优先队列2, 请插入 n 个元素: ");
	scanf("%d", &n);
	input(&h2, n);

	printf("\n合并优先队列1, 2\n");
	h = merge(&h, &h2);

	printf("弹出堆中全部的元素: ");
	while (1)
	{
		data = deletetop(&h);
		if (data != INF)
			printf("%d, ", data);
		else break;
	}
	printf("\n\n");
	destory(h);
	destory(h2);
	return 0;
}
