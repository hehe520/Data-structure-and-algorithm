#include <stdio.h>
#include <malloc.h>

typedef char elemtype;
typedef int status;
#define ERROR 0
#define OK 1

// 结点类型
typedef struct node
{
	elemtype data;
	node *next;
}node;

// 栈的储存结构
typedef struct sqstack
{
	node *top;
	node *base;
	int size;
}sqstack;

status create(sqstack *p)
{
	node *n;

	p->base = NULL;  // 安全性
	n = (node *)malloc(sizeof(node));
	if (!n) return ERROR;
	n->next = NULL;

	p->base = p->top = n;
	p->size = 0;

	return OK;
}

status destory(sqstack *p)
{
	node *del;

	while (p->top)
	{
		del = p->top;
		p->top = p->top->next;
		free(del);
	}
	p->base = p->top = NULL;
	
	return OK;
}

status push(sqstack *p, elemtype *data)
{
	node *n;

	// 先看看栈建好了没有
	if (p->base == NULL) return ERROR;

	// 先赋值，建立一个新结点，连上去
	p->top->data = *data;

	n = (node *)malloc(sizeof(node));
	if (!n) return ERROR;
	n->next = p->top;
	p->top = n;
	p->size++;

	return OK;
}

status pop(sqstack *p, elemtype *back)
{
	node *del;

	// 先看看是不是空栈
	if (p->top == p->base) return ERROR;
	
	// 销毁top结点，再出栈
	del = p->top;
	p->top = p->top->next;
	*back = p->top->data;
	free(del);
	p->size--;

	return OK;
}

elemtype gettop(sqstack *p)
{
	if (p->top == p->base) return ERROR;
	else return p->top->next->data;
}

status ok(elemtype *a, elemtype *b)
{
	if (*a == '(' && *b == ')') return OK;
	else if (*a == '[' && *b == ']') return OK;
	else if (*a == '{' && *b == '}') return OK;
	else return ERROR;
}

status left(elemtype c)
{
	switch (c)
	{
	case '(' :
	case '[' :
	case '{' : return OK;
	default: return ERROR;
	}
}

elemtype anther(elemtype *c)
{
	switch (*c)
	{
	case '(' : return ')';
	case '[' : return ']';
	case '{' : return '}';
	default : return ERROR;
	}
}

int main(void)
{
	sqstack s;
	elemtype c;
	elemtype top;

	create(&s);
	printf("请输入括号：");
	c = getchar();
	while (c != '\n')
	{
		if (left(c)) push(&s, &c);
		else
		{
			top = gettop(&s);
			if (ok(&top, &c)) pop(&s, &top);
			else break;
		}
		c = getchar();
	}
	// c 退出的时候是回车键才正常
	if (s.top == s.base && c=='\n') 
	{
		printf("括号匹配成功！\n");
	}
	else if (s.top == s.base && c!='\n')
	{
		printf("多了 %c\n", c);
	}
	else 
	{
		pop(&s, &top);
		printf("缺少 %c\n", anther(&top));
	}
	destory(&s);

	return 0;
}