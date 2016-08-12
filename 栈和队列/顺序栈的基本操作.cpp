#include <stdio.h>
#include <malloc.h>

typedef int status;
typedef int elemtype;

#define OK     1
#define ERROR  0
#define INC   10   // 增量
#define SIZE  10   // 初始大小

typedef struct stack
{
	elemtype *base;
	elemtype *top;
	int size;
}stack;

// 创建一个空栈
status create(stack *p, int size)
{
	p->base = (elemtype *)malloc(size*sizeof(elemtype));
	if (!p->base) return ERROR;
	p->top = p->base;
	p->size = size;
	return OK;
}

// 入栈
status push(stack *p, elemtype *data)
{
	elemtype *n;
	int s;	// size

	// 看看栈满了没有
	if (p->top - p->base == p->size)
	{
		s = p->size + INC;
		n = (elemtype *)realloc(p->base, s * sizeof(elemtype));
		if (!n) return ERROR;
		p->base = n;
		p->top = p->base + p->size;
		p->size += INC;
		// 安全起见，对 p->top, 重新赋值
	}
	*p->top++ = *data;
	// 上面的话等价于  *(p->top) = data; *p->top++;
	// 先入一个，在 ++ 
	return OK;
}

// 出栈
status pop(stack *p, elemtype *back)
{
	if (p->base == p->top) return ERROR;
	*back = *--p->top;
	// 先--，在出栈顶元素
	return OK;
}

// 返回栈顶元素,不出栈
status gettop(const stack *p, elemtype *back)
{
	if (p->top == p->base) return ERROR;
	*back = *(p->top-1);
	return OK;
}

// 求长度
int lenth(stack *p)
{
	// 看看有没有创建栈，NULL没有
	if (p->base == NULL) return ERROR;
	else return (p->top - p->base);
}

// 判空
status empty(stack *p)
{
	if (p->top == p->base) return OK;
	else return ERROR;
}

// 显示栈中全部元素，不出栈
status onlyshow(const stack *p)
{
	elemtype *cur;

	if (p->top == p->base) return ERROR;
	cur = p->top-1;
	while (cur >= p->base)
	{
		printf("%d\n", *cur);
		cur--;
	}
	return OK;
}

// 清空栈
status clear(stack *p)
{
	if (p->base == NULL) return ERROR;
	p->top = p->base;
	return OK;
}

// 销毁
status destory(stack *p)
{
	if (p->base == NULL) return ERROR;
	free(p->base);
	p->top = p->base = NULL;
	p->size = 0;
	return OK;
}

int main(void)
{
	int i;
	elemtype data;
	stack s;	// 存储位置不动态， 若动态
				// stack *s; s->base=malloc
	if (!create(&s, SIZE))
	{
		printf("初始失败!\n");
		return 0;
	}

	printf("输入5个数据开始入栈：\n");
	for (i=1; i<=20; i++)
	{
		printf("请输入第 %d 个数据：", i); 
		scanf("%d", &data);
		push(&s, &data);
	}
	pop(&s, &data);
	printf("执行出栈操作：%d\n", data);

	gettop(&s, &data);
	printf("栈顶元素：%d\n", data);

	onlyshow(&s);	// 仅查看，不出栈
	
	printf("栈的长度：%d\n", lenth(&s));
	clear(&s);
	printf("栈的长度：%d\n", lenth(&s));

	destory(&s);

	return 0;
}
