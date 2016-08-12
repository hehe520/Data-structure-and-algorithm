
// 高精度浮点型加法, 支持浮点数整数混用
// 计算 x - y = z, 要求 x, y 为正数, 减去一个负数可以转换为加法

#include <stdio.h>
#include <string.h>

#define NUM 100000

// x - y = z
char x[NUM];
char y[NUM];
char z[NUM];

// 高精度比较大小, 返回 a - b 的值
int cmp(char *a, char *b, int len)
{
	int i;

	// 此时位数相等, 逐位比较
	for (i=0; i<len; i++)
	{
		if (a[i] != b[i])
			return a[i] - b[i];
	}
	return 0;		// 相等
}

// 按小数点对齐 a, b 的位数, 如 1.2, 123.456
// 前后补 0 对齐后 001.200,  123.456
void duiqi(char *a, char *b)
{
	int lena = strlen(a);
	int lenb = strlen(b);
	int pa, pb;		// 小数点的位置
	int d;
	int i, j;

	// 确定小数点位置
	for (pa=0; pa<lena; pa++)
		if (a[pa] == '.') break;
	if (a[pa] != '.')		// 整数没有小数点, 人为加上
	{ a[pa] = '.';  a[pa + 1] = '0';  lena += 2; }
	for (pb=0; pb<lenb; pb++)
		if (b[pb] == '.') break;
	if (b[pb] != '.')
	{ b[pb] = '.';  b[pb + 1] = '0';  lenb += 2; }

	// 小数点后面补 0
	d = lenb - pb + pa;
	while (lena < d) a[lena++] = '0';
	d = lena - pa + pb;
	while (lenb < d) b[lenb++] = '0';

	// 整数前面补 0
	if (lena < lenb)
	{
		i = lena - 1;
		j = lenb - 1;
		while (i >= 0) a[j--] = a[i--];
		while (j >= 0) a[j--] = '0';
		lena = lenb;
	}
	if (lenb < lena)
	{
		i = lenb - 1;
		j = lena - 1;
		while (i >= 0) b[j--] = b[i--];
		while (j >= 0) b[j--] = '0';
		lenb = lena;
	}
	a[lena] = b[lenb] = '\0';
}

// 计算 a - b 结果放在 c 中
void sub(char *a, char *b, char *c)
{
	int flag;		// 减后的符号
	int len, i;
	char *change;
	int cf = 0;
	int tmp = 0;
	int topz = NUM;
	int end = NUM - 2;

	// 按小数点对齐 a, b 的位数
	duiqi(a, b);
	// 确定减后的符号, 让 a > b
	len = strlen(a);
	flag = cmp(a, b, len);
	if (flag < 0) 
	{
		change = a;   a = b;   b = change;
		flag = -1;		// 减后是负数
	}
	else if (flag == 0)		// 相等
	{
		z[0] = '0';   z[1] = '\0';
		return ;
	}
	else flag = 1;
	z[--topz] = '\0';
	// 此时 a 的绝对值大于 b, flag 记录符号正负
	// 所以减完后, 不会有借位了
	for (i=len-1; i>=0; i--)
	{
		if (a[i] == '.')
		{
			z[--topz] = '.';
			continue;
		}
		tmp = a[i] - b[i] - cf;
		if (tmp < 0)
		{
			tmp += 10;
			cf = 1;
		}
		else cf = 0;
		z[--topz] = tmp + '0';
	}
	// 消除前导 0, 加上符号
	while (z[topz] == '0') topz++;
	if (z[topz] == '.') z[--topz] = '0';
	if (flag < 0) z[--topz] = '-';
	// 消除后导 0
	while (z[end] == '0') end--;
	// 移到 0 地址
	for (i=0; topz<=end; i++, topz++)
		z[i] = z[topz];
	if (z[i-1] == '.') z[i-1] = '\0';
	else z[i] = '\0';
}

int main(void)
{
	while (1)
	{
		printf("输入 x: ");
		scanf("%s", x);
		printf("输入 y: ");
		scanf("%s", y);
		sub(x, y, z);
		printf("结果: %s\n\n", z);
	}
	return 0;
}