// 动态头结点，合并两个链表
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

node *create(int x)
{
	node *p;
	node *n;

	p = (node *)malloc(sizeof(node));
	if (!p) return ERROR;
	p->next = NULL;

	// p -> [head] -> [1] 
	while (x--)
	{
		n = (node *)malloc(sizeof(node));
		if (!n) return ERROR;
		n->next = p->next;
		p->next = n;
	}
	return p;
}

void input(node *p)
{
	node *cur;
	elemtype data;

	cur = p->next;
	while (cur)
	{
		scanf("%d", &data);
		cur->data = data;
		cur = cur->next;
	}
}

void show(node *p)
{
	node *cur;

	cur = p->next;
	while (cur)
	{
		printf("%-4d", cur->data);
		cur = cur->next;
	}
	printf("\n");
}

void destory(node *p)
{
	node *cur;
	node *del;

	// p -> [head] -> [1] -> [2] 
	cur = p;
	while (cur->next)
	{
		del = cur->next;
		cur->next = del->next;
		free(del);
	}
	free(p);
}

node *add(node *p1, node *p2)
{
	node *c1, *c2, *c3;
	node *p3;

	c1 = p1->next;
	c2 = p2->next;
	c3 = create(0);
	p3 = c3;
	while (c1 && c2)
	{
		if (c1->data <= c2->data)
		{
			c3->next = c1;
			c3 = c3->next;
			c1 = c1->next;
		}
		else 
		{
			c3->next = c2;
			c3 = c3->next;
			c2 = c2->next;
		}
	}
	if (c1) c3->next = c1;
	if (c2) c3->next = c2;

	// p1, p2 的结构已被破坏
	free(p1);
	free(p2);
	return p3;
}

int main(void)
{
	node *list1; 
	node *list2;
	node *list3;

	list1 = create(4);
	list2 = create(4);

	input(list1);
	printf("over\n");
	input(list2);

	show(list1);
	show(list2);

	list3 = add(list1, list2);
	show(list3);

	destory(list3);

	return 0;
}