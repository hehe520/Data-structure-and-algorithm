#include <stdio.h>
#include <malloc.h>

typedef char elemtype;
typedef int status;
#define ERROR 0
#define OK 1
#define SIZE 50

typedef struct 
{
	elemtype *base;
	int top;
}stack;

status create(stack *s)
{
	s->base = (elemtype *)malloc(SIZE * sizeof(elemtype));
	if (!s->base) return ERROR;
	s->top = 0;
	return OK;
}

status destory(stack *s)
{
	if (s && s->base) free(s->base);
	else return ERROR;
	s->top = 0;
	return OK;
}

status push(stack *s, elemtype data)
{
	if (s->top == SIZE) return ERROR;
	s->base[s->top++] = data;
	return OK;
}

status pop(stack *s)
{
	if (s->top) s->top--;
	return OK;
}

elemtype gettop(stack *s)
{
	if (s->top) return s->base[s->top - 1];
	return '\0';
}

inline status left(char a)
{
	if (a == '(' 
		|| a == '{' 
		|| a == '[' 
		|| a == '<')  return OK;
	else return ERROR;
}

status check(char *str)
{
	stack s;
	char a;

	create(&s);	
	while (*str != '\0')
	{
		// ◊Û¿®∫≈»´≤øΩ¯’ª, ”“¿®∫≈Ω¯––’ª∂•∆•≈‰
		if (left(*str)) push(&s, *str);
		else 
		{
			a = gettop(&s);
			if (a == '\0') return ERROR;
			else if (*str == ')' && a == '(') pop(&s);
			else if (*str == '}' && a == '{') pop(&s);
			else if (*str == ']' && a == '[') pop(&s);
			else if (*str == '>' && a == '<') pop(&s);
			else return ERROR;
		}
		str++;
	}
	if (s.top != 0) return ERROR;	// ’ªø’¡À, ≤≈∆•≈‰≥…π¶
	destory(&s);
	return OK;
}

int main(void)
{
	char str[100];

	printf("¿®∫≈∆•≈‰ºÏ≤‚, ÷ß≥÷ (),[],{},<>\n«Î ‰»Î: ");
	scanf("%s", str);
	
	if (check(str)) printf("∆•≈‰≥…π¶!\n");
	else printf("∆•≈‰ ß∞‹!\n");

	return 0;
}