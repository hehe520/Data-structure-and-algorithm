#include <stdlib.h>
#include <stdio.h>
#include "stack.h"

int number(char c)
{
	if ((c >= '0' && c <= '9') 
		|| (c == '.')) return 1;
	else return 0;
}

int cmp(char op, char b)
{
	// 运算优先级
	// 最高是 (
	// 过来是 * / 
	// 过来是 + -
	// 过来是 )
	// 最后是 #
	// 1 是执行运算，0 是进栈
	if (b == '(') return 0;
	else 
	{
		if (op == '*' || op == '/') return 1;
		else if ((op == '+' || op == '-') 
			&& (b != '*' && b != '/')) return 1;
		else return 0;
	}
}

float cal(float a, char c, float b)
{
	switch (c)
	{
	case '+' : return a + b; break;
	case '-' : return a - b; break;
	case '*' : return a * b; break;
	case '/' : return a / b; break;
	default : return 0;
	}
}

int error(char c)
{
	if (c == '+' 
		||c == '-' 
		||c == '*' 
		||c == '/') return 0;
	else return 1;
}

// 0. 先把 # 压入栈，再人为的在表达式后面加上#
// 1. 若当前是数字就进数字栈
// 2. 若不是数字，就比较优先级
// 3. 优先级高就出栈运算，完了，结果再入栈
// 4. 优先级低就操作符入操作符栈
// 5. 直到操作符栈为空，结束循环
// 6. 操作符栈不空, 表达式错误
// 7. 去数字栈里面取最终结果
status calc(char *s, float *back)
{
	opstack op;		// 操作符栈
	numstack num;	// 数字栈
	int pos = 0;
	float f;
	float a, b;
	char flag;

	opcreate(&op);
	numcreate(&num);
	oppush(&op, '#');	// 加# 方便处理，
	while (s[pos] != '\0') pos++;	// 在后面也加 #
	s[pos] = '#';
	s[pos + 1] = '\0';
	pos = 0;
	while (op.base != op.top)
	{
		// 看看是数字还是字符
		if (number(s[pos]))
		{
			sscanf(s+pos, "%f", &f);
			while (number(s[pos])) pos++;
			numpush(&num, f);
		}
		else 
		{
			// 比较优先级
			if (cmp(opgettop(&op), s[pos]))
			{
				// 出来计算
				if (!numpop(&num, &a)) break;
				if (!numpop(&num, &b)) break;
				if (!oppop(&op, &flag)) break;
				f = cal(b, flag, a);
				numpush(&num, f);
			}
			else 
			{
				// s[pos] 进栈, pos++
				if (s[pos] != '\0')
				{
					oppush(&op, s[pos++]);
				}
				// 刷新一下括号匹配
				flag = opgettop(&op);
				if (flag == ')' || flag == '#')
				{
					// 删掉一对括号
					oppop(&op, &flag);
					oppop(&op, &flag);
				}
			}
		}
	}
	if (op.top == op.base)
	{
		numpop(&num, back);
		opdestory(&op);
		numdestory(&num);
		return OK;
	}
	else 
	{
		opdestory(&op);
		numdestory(&num);
		return ERROR;
	}
}

int main(void)
{
	char buf[200] = {0};
	int flag;
	float back;

	while (1)
	{
		printf("\n--- 表达式求值, 仅支持四则运算\n");
		printf("\n--- 请输入一个表达式: ");
		gets(buf);
		flag = 0;
		if (buf[0] != '\0')
		{
			flag = calc(buf, &back);
			if (flag)
			{
				printf("\n--- 计算结果:  %g\n", back);
			}
			else printf("\n--- 表达式错误!\n");
		}
		else printf("\n--- 没有输入!\n");
		printf("\n--- 按回车键继续计算...");
		fflush(stdin);
		getchar();
		system("cls");
	}
	return 0;
}