
// str_to_int();		字符串转整型
// int_to_str();		整型转字符串
// str_to_double();		字符串转浮点型
// double_to_str();		浮点型转字符串

#include <stdio.h>

// 字符串转整型, 支持正负号
int str_to_int(char *str)
{
	int flag;
	char *cur = str;
	int r = 0;

	// 判断符号
	if (*str == '-') { flag = -1;   cur = str + 1; }
	else if (*str == '+') { flag = 1;   cur = str + 1; }
	else flag = 1;

	while (*cur != '\0')
	{
		r = r * 10 + *cur - '0';
		cur++;
	}
	return r * flag;
}

// 整型转字符串, *str 返回结果, 支持负数
void int_to_str(int a, char *str)
{
	char *cur = str;
	char stack[20];		// int 型最多 10 位数
	int top = 0;

	if (a < 0) { *cur++ = '-';   a *= -1; }
	do
	{
		stack[top++] = a % 10 + '0';
		a /= 10;
	}while (a != 0);
	while (top != 0)
	{
		*cur++ = stack[--top];
	}
	*cur = '\0';
}

// 字符串转浮点型, 支持正负号
double str_to_double(char *str)
{
	int flag = -1;
	char *cur = str;
	double r = 0.0;
	int point = 0;		// 判断小数点在前还是后
	double d;			// 乘量

	// 判断符号
	if (*str == '-') { flag = -1;   cur = str + 1; }
	else if (*str == '+') { flag = 1;   cur = str + 1; }
	else flag = 1;

	while (*cur != '\0')
	{
		if (*cur == '.')		// 遇到小数点
		{
			point = 1;
			d = 1;
			cur++;
		}
		if (point) 
		{
			d *= 0.1;
			r += (*cur - '0') * d;
		}
		else 
		{
			r = r * 10 + *cur - '0';
		}
		cur++;
	}
	return r * flag;
}

// 浮点型转字符串, 支持负数
// 算法: 将浮点型从小数点的位置拆成两半
// 如 123.456 拆成 123 与 0.456
// 123 调用 int_to_str 处理
// 0.456 乘 10 取整处理,  num 表示保留的小数位数
void double_to_str(double f, char *str, int num)
{
	char *cur = str;
	int a;
	int i;

	if (f < 0) { *cur++ = '-';   f *= -1; }
	a = (int)f;
	f -= (double)a;
	int_to_str(a, cur);
	while (*cur != '\0') cur++;
	*cur++ = '.';
	for (i=0; i<num; i++)
	{
		f *= 10;
		a = (int)f;
		f -= a;
		*cur++ = a + '0';
	}
	*cur = '\0';
}

int main(void)
{
	char str[100];
	int a;
	double f;

	// 注意数据不要超出 int, double 的表示范围
	// 大数据可以采用高精度
	while (1)		// 测试 str_to_int()
	{
		printf("字符串转整型: ");
		scanf("%s", str);
		a = str_to_int(str);
		printf("整型: %d\n", a);
	}
	while (1)		// 测试 int_to_str()
	{
		printf("整型转字符串: ");
		scanf("%d", &a);
		int_to_str(a, str);
		printf("字符串: %s\n", str);
	}
	while (1)		// 测试 str_to_double()
	{
		printf("字符串转浮点型: ");
		scanf("%s", str);
		f = str_to_double(str);
		printf("浮点型: %lf\n", f);
	}
	while (1)		// 测试 double_to_str()
	{
		printf("浮点型转字符串: ");
		scanf("%lf", &f);
		double_to_str(f, str, 6);
		printf("字符串: %s\n", str);
	}
	return 0;
}