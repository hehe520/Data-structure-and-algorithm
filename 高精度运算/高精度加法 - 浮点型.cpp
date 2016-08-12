
// 高精度浮点型加法
// input()		输入
// show()		输出
// add()		相加

#include <stdio.h>
#include <string.h>

#define NUM 1000000		// 100 万位, 约占 5.7MB 内存

// n[] 储存小数点前的数, _n 储存小数点后的数
// 从地址 0 开始存起
typedef struct
{
	char n[NUM];
	int  len;
	char _n[NUM];
	int  _len;
}bignum;			// 大整数

bignum x, y, z;			// x + y = z

// 输入模块
void input(bignum *str)
{
	char tmp;
	int point = 1;
	
	str->len = str->_len = 0;
	while (1)
	{
		tmp = getchar();
		if (tmp == '\n') break;
		if (point)
		{
			if (tmp == '.') point = 0;
			else str->n[str->len++] = tmp;
		}
		else str->_n[str->_len++] = tmp;
	}
	str->n[str->len] = '\0';
	str->_n[str->_len] = '\0';
}

// 输出模块, num 表示输出的小数位, num == -1 表示全部输出
void show(bignum *s, int num)
{
	char tmp;
	int i;

	printf("%s", s->n);
	if (s->_len != 0 && num != 0) putchar('.');
	if (num <= s->_len)
	{
		// 频繁调用 printf 很费时
		tmp = s->_n[num];
		s->_n[num] = '\0';
		printf("%s", s->_n);
		s->_n[num] = tmp;
	}
	else
	{
		printf("%s", s->_n);
		for (i=num - s->_len; i>=0; i--)
			putchar('0');
	}
	putchar('\n');
}

// 整型加法模块, xx + yy = zz
// 返回进位标志 cf, flag 标志计算整数部分还是小数
int int_add(char *xx, char *yy, char *zz, int cf, int flag)
{
	int len1 = strlen(xx);
	int len2 = strlen(yy);
	int min = len1 > len2 ? len2 : len1;
	int top = NUM;
	int tmp;
	int a, b, i;

	// 频繁调用 printf() 花时间, 用 z 保存结果, 一次输出
	zz[--top] = '\0';
	for (i=min-1; i>=0; i--)
	{
		a = xx[--len1] - '0';
		b = yy[--len2] - '0';
		tmp = (a + b + cf) % 10;
		cf = (a + b + cf) / 10;
		zz[--top] = tmp + '0';
	}
	if (len1) 
	{
		while (cf != 0)
		{
			if (len1 > 0)
				a = xx[--len1] - '0';
			else a = 0;
			tmp = (a + cf) % 10;
			zz[--top] = tmp + '0';
			cf = (a + cf) / 10;
		}
		while (len1) 
			zz[--top] = xx[--len1];
	}
	else if (len2)
	{
		while (cf != 0)
		{
			if (len2 > 0)
				a = yy[--len2] - '0';
			else a = 0;
			tmp = (a + cf) % 10;
			zz[--top] = tmp + '0';
			cf = (a + cf) / 10;
		}
		while (len2) 
			zz[--top] = yy[--len2];
	}
	i = 0;
	if (flag == 1 && cf == 1) zz[i++] = '1';
	while (1)		// 移动到 0 地址
	{
		zz[i++] = zz[top++];
		if (zz[top-1] == '\0') break;
	}
	return cf;
}

// 浮点数相加运算 x + y = z
// 先把小数部分的位数补齐, 调用整型加法模块相加, 返回进位标志
// 整数部分直接调用整型加法模块进行相加, 同时加上小数的进位
void add(void)
{
	int i;
	int cf = 0;		// 进位标志

	// 小数部分补齐 0
	if (x._len < y._len)
	{
		for (i=x._len; i<y._len; i++)
			x._n[i] = '0';
		x._n[i] = '\0';
		x._len = y._len;
	}
	else if (y._len < x._len)
	{
		for (i=y._len; i<x._len; i++)
			y._n[i] = '0';
		y._n[i] = '\0';
		y._len = x._len;
	}
	// 小数部分相加
	cf = int_add(x._n, y._n, z._n, cf, 0);
	z._len = x._len;
	while (z._n[z._len - 1] == '0') z._len--;	// 消除后导 0
	z._n[z._len] = '\0';
	// 整数部分相加
	cf = int_add(x.n, y.n, z.n, cf, 1);
}

int main(void)
{
	while (1)
	{
		// 输入两个正数, 若加上一个负数, 可以转换为减法
		// 支持浮点数和整数混用
		printf("\n输入两个数:\n第一个: ");
		input(&x);
		printf("第二个: ");
		input(&y);
		add();
		printf("结果 :  ");
		show(&z, -1);		// 输出全部小数
//		show(&z, 6);		// 输出 6 位小数
//		show(&z, 0);		// 取整
	}
	return 0;
}