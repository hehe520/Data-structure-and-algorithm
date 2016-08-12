
// 计算 x - y = z
// x 和 y 都要是正数, 减去一个负数可以转化为高精度加法

#include <stdio.h>
#include <string.h>

#define NUM 1000000

char x[NUM], y[NUM], z[NUM];
int lenx, leny, lenz;

// 计算 x - y
// 比较两个大数的大小, 确定符号
// 用绝对值大的减去小的, 再加上符号
void sub(void)
{
	int flag;
	char *a, *b;		// 用 a - b
	int lena, lenb;
	int tmp, i, min;
	int cf = 0;			// 借位标志

	lenx = strlen(x);
	leny = strlen(y);
	lenz = NUM;
	z[--lenz] = '\0';
	if (lenx == leny)
	{
		flag = strcmp(x, y);		// flag 有 1, 0, -1 三种
		if (flag == 0)
		{
			z[--lenz] = '0';
			return ;
		}
	}
	else flag = (lenx > leny);		// flag == 1 是正数, 其它是负数
	if (flag == 1) { a = x;  lena = lenx;  b = y;  lenb = leny; }
	else { a = y;  lena = leny;  b = x;  lenb = lenx; }

	// a 是大于 b 的, 所以循环到 lenb
	for (i=0, min=lenb; i<min; i++)
	{
		tmp = a[--lena] - b[--lenb] - cf;
		if (tmp < 0)
		{
			tmp += 10;
			cf = 1;
		}
		else cf = 0;
		z[--lenz] = tmp + '0';
	}
	if (lena)
	{
		// 10000 - 9 需要一直向前借位
		while (lena && cf)
		{
			tmp = a[--lena] - '0' - cf;
			if (tmp < 0)
			{
				tmp += 10;
				cf = 1;
			}
			else cf = 0;
			z[--lenz] = tmp + '0';
		}
		while (lena) z[--lenz] = a[--lena];
	}
	// 因为前面保证了大数减小数, 所以最后 cf 不会有借位
	while (z[lenz] == '0') lenz++;			// 消除前导 0
	if (flag != 1) z[--lenz] = '-';
}

int main(void)
{
	while (1)
	{
		printf("\n输入 x: ");
		scanf("%s", x);
		printf("输入 y: ");
		scanf("%s", y);
		sub();		// 计算 x - y
		printf("结果  : %s\n", z + lenz);
	}
	return 0;
}