#include <stdio.h>
#include <malloc.h>
#include "stack.h"

status opcreate(opstack *p)
{
	p->base = (cnode *)malloc(sizeof(cnode));
	if (!p->base) return ERROR;
	p->base->next = NULL;
	p->top = p->base;
	p->lenth++;

	return OK;
}

status opdestory(opstack *p)
{
	cnode *del;

	if (!p->base) return ERROR;
	while (p->top)
	{
		del = p->top;
		p->top = del->next;
		free(del);
	}
	p->top = p->base = NULL;
	p->lenth = 0;
	return OK;
}

status oppush(opstack *p, char data)
{
	cnode *n;

	if (!p->base) return ERROR;
	p->top->data = data;	// 先赋值, 再top++
	n = (cnode *)malloc(sizeof(cnode));
	if (!n) return ERROR;
	n->next = p->top;		// 连在 top 的前面
	p->top = n;

	return OK;
}

status oppop(opstack *p, char *back)
{
	cnode *del;

	if (!p->base) return ERROR;
	// 先top--. 再取值
	del = p->top;
	p->top = del->next;
	free(del);
	if (p->top == NULL) return ERROR;
	*back = p->top->data;

	return OK;
}

char opgettop(opstack *p)
{
	if (p->top == p->base) return ERROR;
	return (p->top->next->data);
}

// ----------下面是数字栈-------------

status numcreate(numstack *p)
{
	p->base = (nnode *)malloc(sizeof(nnode));
	if (!p->base) return ERROR;
	p->base->next = NULL;
	p->top = p->base;
	p->lenth++;

	return OK;
}

status numdestory(numstack *p)
{
	nnode *del;

	if (!p->base) return ERROR;
	while (p->top)
	{
		del = p->top;
		p->top = del->next;
		free(del);
	}
	p->top = p->base = NULL;
	p->lenth = 0;
	return OK;
}

status numpush(numstack *p, float data)
{
	nnode *n;

	if (!p->base) return ERROR;
	p->top->data = data;	// 先赋值, 再top++
	n = (nnode *)malloc(sizeof(nnode));
	if (!n) return ERROR;
	n->next = p->top;		// 连在 top 的前面
	p->top = n;

	return OK;
}

status numpop(numstack *p, float *back)
{
	nnode *del;

	if (!p->base) return ERROR;
	// 先top--. 再取值
	del = p->top;
	p->top = del->next;
	free(del);
	if (p->top == NULL) return ERROR;
	*back = p->top->data;

	return OK;
}

float numgettop(numstack *p)
{
	if (p->top == p->base) return ERROR;
	return (p->top->next->data);
}