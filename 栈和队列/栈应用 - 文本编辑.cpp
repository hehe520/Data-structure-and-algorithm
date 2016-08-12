#include <stdio.h>
#include <malloc.h>

typedef int status;
typedef char elemtype;

#define OK 1
#define ERROR 0

// 采用单向链表结构
typedef struct node
{
	elemtype data;
	node *next;
}node;

// 头结构
typedef struct sqstack
{
	node *top;
	node *base;
	int lenth;
}sqstack;

status create(sqstack *p)
{
	node *n;

	// 初始的时候就放入一个结点，
	n = (node *)malloc(sizeof(node));
	if (!n) return ERROR;
	n->next = NULL;

	p->base = p->top = n;
	p->lenth = 0;
	return OK;
}

// 入栈，先赋值，后生成一个新结点，连到top处
status push(sqstack *p, elemtype *data)
{
	node *n;

	p->top->data = *data;	// 类似 *p->top = *data

	n = (node *)malloc(sizeof(node));
	if (!n) return ERROR;
	n->next = p->top;		// 类似 p->top++;
	p->top = n;
	p->lenth++;

	return OK;
}

// 出栈，先销毁top结点，top--, 后出栈
status pop(sqstack *p, elemtype *back)
{
	node *del;

	// 看看是不是空栈
	if (p->top == p->base) return ERROR;
	del = p->top;			// 待删除的结点
	p->top = p->top->next;	// 相当于 p->top--
	*back = p->top->data;   // 相当于 *back = *p->top;
	free(del);
	p->lenth--;
	
	return OK;
}

// 仅查看，不出栈
status onlyshow(sqstack *p)
{
	node *cur;
	
	if (p->top == p->base) return ERROR; // 空栈
	cur = p->top->next;
	while (cur)
	{
		printf("--- %d\n", cur->data);
		cur = cur->next;
	}
	return OK;
}

// 返回栈顶元素,不出栈
status getelem(sqstack *p, elemtype *data)
{
	if (p->top == p->base) return ERROR;
	*data = p->top->next->data;
	
	return OK;
}

// 求长度
int lenth(sqstack *p)
{
	return p->lenth;
}

// 判空
status emptystack(sqstack *p)
{
	if (p->top == p->base) return ERROR;
	else return OK;
}

// 清空栈, 链表中写0，不销毁
status clear(sqstack *p)
{
	node *cur;
	
	if (p->top == p->base) return ERROR;
	cur = p->top->next;
	while (cur)
	{
		cur->data = 0;
		cur = cur->next;
	}
	return OK;
}

// 从头销毁
status destory(sqstack *p)
{
	node *del;

	if (p->base == NULL) return ERROR;
	while (p->top != p->base)
	{
		del = p->top;
		p->top = p->top->next;
		free(del);
	}
	free(p->base);
	return OK;
}

int main(void)
{
	sqstack s; 
	sqstack nixu;
	char c;
	int count;

	create(&nixu);
	create(&s);
	printf("请输入英文句子：(退格符 #, 回车键结束输入)\n");

	c = getchar();
	while (c != '\n')
	{
		push(&s, &c);
		c = getchar();
	}
	while (getelem(&s, &c))
	{
		if (c == '#') 
		{
			// 看看有多少个 #, 就要消掉多少个字符
			count = 0;
			while (1)
			{
				getelem(&s, &c);
				if (c == '#')
				{
					pop(&s, &c);
					count++;
				}
				else break;
			}
			while (count--) pop(&s, &c);
		}
		else 
		{
			// 由于栈是后出的，所以用一个辅助栈，
			pop(&s, &c);
			push(&nixu, &c);
		}
	}
	while (pop(&nixu, &c)) printf("%c", c);
	printf("\n");
	destory(&s);
	destory(&nixu);

	return 0;
}
