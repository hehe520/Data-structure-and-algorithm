
// 高精度乘法, 大整数乘以大整数, 不支持负数小数
// 算法: 模拟人工计算的过程
// 例如 123456 * 789  依次用 9, 8, 7 乘以 123456
// 将中间结果相加即可, 所以需要高精度加法模块

// 分析过程:
// 因为 123456 * 789 = 123456 * 780 + 123456 * 9
// 人工计算是一位一位拆的, 一个数字有 n 位, 那么要拆 n 次
// 在 int 能表示的范围内, 一次可以拆 4 位, 那么要拆 n/4 次
// 因为来自键盘输入流的是字符, 所以还得每 4 位转换成整型
// 一个 int 保存 4 位数字

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <limits.h>

#define NUM 1000000		// 精度 10^5000000

// x + y = z 用 int 数组逆序保存, 用 len 指示长度
int x[NUM], lenx;
int y[NUM], leny;
int z[2 * NUM], lenz;
// 为了节约内存, x, y, z 可以改为动态申请
// 类似顺序表那样根据实际使用动态增长, 但需要额外增加代码

// 输入模块, 每 4 位转换成整型
void input(int *base, int *len)
{
	char *buf;
	int lenb, i, cur;
	int tmp;
	int flag = 1;

	*len = 0;
	buf = (char *)malloc(4 * NUM * sizeof(char));
	if (buf == NULL) return ;
	scanf("%s", buf);
	lenb = strlen(buf);
	while (lenb >= 4)
	{
		cur = lenb - 4;
		for (tmp=i=0; i<4; i++)
			tmp = tmp * 10 + buf[cur++] - '0';
		base[(*len)++] = tmp;
		lenb -= 4;
	}
	if (lenb)
	{
		for (tmp=i=0; i<lenb; i++)
			tmp = tmp * 10 + buf[i] - '0';
		base[(*len)++] = tmp;
	}
	free(buf);
}

void show(int *base, int len)		// 输出模块
{
	int i;;

	printf("%d", base[len - 1]);
	for (i=len-2; i>=0; i--)
		printf("%04d", base[i]);
	putchar('\n');
}

// 将高精度 a 与低精度 b 相乘, 结果放在 c 中
void _mul(int *a, int lena, int b, int *c, int *top)
{
	int tmp, i;
	int cf = 0;

	for (i=0; i<lena; i++)
	{
		tmp = b * a[i] + cf;
		c[(*top)++] = tmp % 10000;
		cf = tmp / 10000;
	}
	if (cf) c[(*top)++] = cf;
}

// 将高精度 a, b 相加, 结果放在 b 中
void _add(int *a, int lena, int *b, int *lenb)
{
	int i;
	int tmp, cf;

	// 通过高位补 0 将 a, b 的位数补齐
	while (lena < *lenb) a[lena++] = 0;
	while (*lenb < lena) b[(*lenb)++] = 0;
	for (cf=i=0; i<lena; i++)
	{
		tmp = a[i] + b[i] + cf;
		b[i] = tmp % 10000;
		cf = tmp / 10000;
	}
	if (cf) b[(*lenb)++] = cf;
}

// 计算 a * b 结果保存在 c 中
void mul(int *a, int lena, int *b, int lenb, int *c, int *lenc)
{
	int i, j;
	int t, *tmp;
	int top;		// tmp 和 top 记录中间结果

	if (lena < lenb)	// 保证 a 的位数大
	{
		t = lena;   lena = lenb;   lenb = t;
		tmp = a;   a = b;   b = tmp;
	}
	*lenc = 0;
	tmp = (int *)malloc(2 * NUM * sizeof(int));
	if (tmp == NULL) return ;
	for (i=0; i<lenb; i++)		// 用 b 来乘 a
	{
		for (top=j=0; j<i; j++) tmp[top++] = 0;
		_mul(a, lena, b[i], tmp, &top);
		_add(tmp, top, c, lenc);
		// _mul 高精度乘低精度模块, 产生 tmp 中间结果
		// _add 中间结果 tmp 累加到 c 中
	}
	free(tmp);
}

int main(void)
{
	while (1)
	{
		printf("\n输入大整数 x: ");
		input(x, &lenx);
		printf("输入大整数 y: ");
		input(y, &leny);
		mul(x, lenx, y, leny, z, &lenz);	// 计算 x * y = z
		printf("结果: ");
		show(z, lenz);
	}
	return 0;
}
