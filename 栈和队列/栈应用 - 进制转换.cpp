// 栈应用，数值转换，顺序栈存储
#include <stdio.h>
#include <malloc.h>

typedef int status;
typedef int elemtype;

#define OK     1
#define ERROR  0
#define SIZE   10
#define INC    10

// 顺序栈存储结构
typedef struct
{
	elemtype *base;		// base 指向一大块空间
	elemtype *top;
	int size;
}stack;

status create(stack *p)
{
	p->base = (elemtype *)malloc(SIZE*sizeof(elemtype));
	if (!(p->base)) return ERROR;
	p->top = p->base;
	p->size = SIZE;

	return OK;
}

status destory(stack *p)
{
	if (!(p->base)) return ERROR;
	free(p->base);

	p->base = p->top = NULL;
	p->size = 0;

	return OK;
}

status push(stack *p, elemtype data)
{
	elemtype *n;
	int newsize;  //为了一行不写太多代码
	
	// 看看栈建好了没有
	if (!(p->base)) return ERROR;
	
	// 看看空间够不够
	if (p->top - p->base == p->size)
	{
		newsize = p->size + INC;
		n = (elemtype *)realloc(p->base, newsize* sizeof(elemtype));
		if (!n) return ERROR;
		p->base = n;
		p->size = newsize;
	}

	// 开始入栈
	*p->top = data;
	p->top++;

	return OK;
}

status pop(stack *p, elemtype *back)
{
	// 看看是不是建好的栈
	if (p->base == NULL) return ERROR;
	// 看看是不是空栈
	if (p->top == p->base) return ERROR;

	p->top--;
	*back = *p->top;

	return OK;
}

char inttochar(int n)
{
	if (n>=0 && n<=9) return (n + '0');
	else return (n - 10 + 'A');
}

int main(void)
{
	stack s;
	elemtype n;
	int r;

	printf("十进制转 R 进制，(2 <= R <= 16)\n");
	printf("请输入一个十进制正数：");
	scanf("%d", &n);
	if (n<=0) return 0;
	printf("请输入 R：");
	scanf("%d", &r);
	if (r<2 || r>16) return 0;

	// 开始转换
	create(&s);
	while (n)
	{
		push(&s, n % r);
		n /= r;
	}
	// 全部出栈并显示
	while (pop(&s, &n))
	{
		printf("%c", inttochar(n));
	}
	printf("\n");
	destory(&s);
	
	return 0;
}
