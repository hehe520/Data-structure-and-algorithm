#include <stdio.h>
#include <malloc.h>

typedef int status;
typedef int elemtype;

#define OK 1
#define ERROR 0

typedef struct node 
{
	elemtype data;
	node *next;
}node;

status create(node *n, int num)
{
	node *cur;

	n->next = NULL;
	n->data = 0;
	while (num--)
	{
		cur = (node *)malloc(sizeof(node));
		if (!cur) return ERROR;
		cur->next = n->next;
		n->next = cur;
		n->data++;
	}

	return OK;
}

status input(node *n)
{
	node *cur;

	cur = n->next;
	while (cur)
	{
		scanf("%d", &cur->data);
		cur = cur->next;
	}
	return OK;
}

status show(node *n)
{
	node *cur;

	cur = n->next;
	while (cur)
	{
		printf("%d, ", cur->data);
		cur = cur->next;
	}
	printf("\n");

	return OK;
}

status destory(node *n)
{
	node *cur;

	cur = n->next;
	while (cur)
	{
		n->next = cur->next;
		free(cur);
		cur = n->next;
	}
	return OK;
}

// 头插法，逆置链表
status linklist_stv(node *n)
{
	// [head] [1] [2]
	// 把 2 后面的，全部跑到 1 前面

	node *cur;
	node *pos;

	cur = n->next->next;
	n->next->next = NULL;
	while (cur)
	{
		pos = cur->next;	// 抓住 cur 的后面链
		cur->next = n->next;	// cur 头插
		n->next = cur;
		cur = pos;		// 拿回后面条链
	}
	return OK;
}

// 改变指针指向, 逆置链表
status linklist_stv2(node *n)
{
	// [head] -> [1]   -> [2]   -> [3] -> NULL
	// [head] -> [cur] <- [pos] .. [rear] -> NULL
	node *cur;
	node *pos;
	node *rear;

	cur = n->next;
	pos = cur->next;
	rear = pos->next;

	cur->next = NULL; // 记得封口NULL
	while (1)
	{
		pos->next = cur;
		if (!rear) break;
		cur = pos;
		pos = rear;
		rear = rear->next;
	}
	n->next = pos;	// 头结点指向末尾

	return OK;
}

int main(void)
{
	node n;

	create(&n, 10);
	input(&n);
	show(&n);

	linklist_stv(&n);
	show(&n);
	
	linklist_stv2(&n);
	show(&n);

	destory(&n);

	return 0;
}