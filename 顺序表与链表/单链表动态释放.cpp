#include <stdio.h>
#include <malloc.h>

typedef int status;
typedef int elemtype;

#define OK         1
#define ERROR      0

typedef struct node
{
	elemtype data;
	node *next;
}node;

node *create(int n)
{
	node *p, *b;

	p = (node *)malloc(sizeof(node));
	p->next = NULL;
	if (!p) return ERROR;
	// p -> [head] -> [1]
	while (n--)
	{
		b = (node *)malloc(sizeof(node));
		if (!b) return ERROR;
		b->next = p->next;
		p->next = b;
	}
	return p;
}

void destory(node *p)
{
	node *cur;
	node *del;

	// [p] -> [1] -> [2]
	cur = p;
	while (cur->next)
	{
		del = cur->next;
		cur->next = del->next;
		free(del);
	}
	free(p);
}

int main(void)
{
	node *p;
	node *cur;
	elemtype num = 0;

	p = create(6);
	cur = p;
	while (cur->next)
	{
		cur->data = num++;
		cur = cur->next;
	}

	cur = p;
	while (cur->next)
	{
		printf("%-4d", cur->data);
		cur = cur->next;
	}
	printf("\n");

	destory(p);

	return 0;
}