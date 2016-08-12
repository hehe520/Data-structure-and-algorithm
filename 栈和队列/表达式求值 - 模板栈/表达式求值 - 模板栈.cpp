#include <iostream>
#include <stdio.h>
#include <string.h>
#include "my_stack.h"	// 提供栈模板
using namespace std;


// 比较运算符优先级
// true 是出栈运算,  false 是入栈
inline bool cmp(char old, char n)
{
	// ()的优先级最高
	if (n == '(') return false;
	if (n == ')' && old != '(') return true;
	if (n == ')' && old == '(') return false;

	// 乘除法优先级第二
	if (old == '*' || old == '/') return true;
	if (n == '*' || n == '/') return false;

	// 比较加减法
	if (old == '+' || old == '-') return true;
	else return false;
}

// 判断是不是数字
inline bool number(char ch)
{
	if (ch >= '0' && ch <= '9') return true;
	if (ch == '.') return true;
	return false;
}

double result(double a, char flag, double b)
{
	switch (flag)
	{
	case '+' : return a + b;
	case '-' : return a - b;
	case '*' : return a * b;
	case '/' : return a / b;
	}
	return 0;
}

// 如果是数字, 则进栈,
// 如果是运算符则比较优先级
// 知道操作符栈为空
double calc(char *str)
{
	stack<char> OP;		// 操作符栈
	stack<double> NUM;	// 数字栈
	int pos;
	double real, real2;
	char ch;

	OP.push('#');	// 方便处理运算符
	pos = strlen(str);
	str[pos++] = '#';
	str[pos] = '\0';
	pos = 0;
	while (!OP.empty())
	{
		if (number(str[pos]))	// 如果是数字
		{
			sscanf(str+pos, "%lf", &real);
			NUM.push(real);
			// 消去 str 中的数字
			while (number(str[pos])) pos++;
		}
		else	// 如果是字符
		{
			// 比较优先级
			if (cmp(OP.top(), str[pos]))
			{
				// OP 里的优先级高, 出栈运算
				real2 = NUM.top();   NUM.pop();
				real = NUM.top();   NUM.pop();
				ch = OP.top();   OP.pop();
				// 运算结果压回栈
				NUM.push(result(real, ch, real2));
			}
			else 
			{
				// 刷新一对括号, 或入栈
				if (str[pos] == ')' && OP.top() == '(') OP.pop();
				else if (str[pos] == '#' && OP.top() == '#') OP.pop();
				else if (str[pos] != '\0')  OP.push(str[pos]);
				else break;
				pos++;
			}
		}
	}
	// 操作符栈空了,表示计算成功
	if (OP.empty()) return NUM.top();
	return 0;
}

int main()
{
	char str[150];
	double result = 1.154541654162;

	while (1)
	{
		system("cls");
		cout << endl;
		cout << "表达式求值 --- 仅支持带括号的四则运算";
		cout << endl << endl;
		cout << "请输入表达式: ";
		cin >> str;
		cout << endl << "计算结果: ";
		cout << calc(str) << endl << endl;
		system("pause");
	}
	return 0;
}
