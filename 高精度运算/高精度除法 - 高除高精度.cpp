
// 高精度除法, 高精度除以高精度
// 在试商的时候, 不断地用减法确定商
// 所以还需要高精度减法模块

#include <stdio.h>
#include <string.h>
#include <malloc.h>

#define NUM 1000000

// 计算 a / b
char a[NUM];		// 除数
char b[NUM];		// 被除数
char c[NUM];		// 商
char r[NUM];		// 余数

// 高精度比较大小, 返回 a - b 的值
// 比较范围是 [sa..ea] 和 [sb..eb]
int cmp(char *a, int sa, int ea, char *b, int sb, int eb)
{
	int i, j;
	int lena = ea - sa;
	int lenb = eb - sb;

	if (lena < lenb) return -1;
	else if (lenb < lena) return 1;
	// 此时位数相等, 逐位比较
	for (i=sa, j=sb; i<ea; i++, j++)
	{
		if (a[i] != b[j])
			return a[i] - b[j];
	}
	return 0;		// 相等
}

// 清除前导 0
void clear_zero(char *a)
{
	int i, j;
	int len = strlen(a);

	for (i=0; i<len; i++)
	{
		if (a[i] != '0') break;
	}
	if (i != 0)
	{
		for (j=0; i<len; i++, j++)
			a[j] = a[i];
		if (j == 0) {a[j] = '0';  a[j + 1] = '\0';}
		else a[j] = '\0';
	}
}

// 高精度减法 a = a - b,  a >= b 不会出现负数
void sub(char *a, int left, int right, char *b)
{
	int lenb = strlen(b);
	int i;
	int tmp;
	int topa = right;
	int cf = 0;

	for (i=lenb-1; i>=0; i--)
	{
		tmp = a[--topa] - b[i] - cf;
		if (tmp < 0)
		{
			tmp += 10; 
			cf = 1;
		}
		else cf = 0;
		a[topa] = tmp + '0';
	}
	if (cf) a[--topa]--;
}

// 计算 a / b, 出口参数 c, r, 返回成功标志
int div(char *a, char *b, char *c, char *r)
{
	int f, s, i;
	int lena, lenb;
	char *tmp;
	int left, right;	// tmp 的工作区间
	int topa, topc;

	clear_zero(a);		// 清除前导 0
	clear_zero(b);
	lena = strlen(a);	// 比较 a, b 大小
	lenb = strlen(b);
	f = cmp(a, 0, lena, b, 0, lenb);	// 返回 a - b
	if (f < 0)			// a < b
	{
		c[0] = '0';   c[1] = '\0';
		strcpy(r, a);
		return 1;
	}
	else if (f == 0)	// a == b
	{
		c[0] = '1';   c[1] = '\0';
		r[0] = '0';   r[1] = '\0';
		return 1;
	}
	// 此时 a > b 且 lena >= lenb
	tmp = (char *)malloc(lena * sizeof(char) + 10);
	if (tmp == NULL) return 0;		// 内存不足

	// 准备试商的部分复制到 tmp
	for (i=0; i<lenb; i++) tmp[i] = a[i];
	topc = left = 0;
	topa = right = lenb;
	while (1)
	{
		s = 0;
		// 如果 tmp >= b 连续高精度减法, 确定商 s
		while (cmp(tmp, left, right, b, 0, lenb) >= 0)
		{
			// tmp 减去 b, 结果放在 tmp 中
			sub(tmp, left, right, b);
			while (left < right && tmp[left] == '0') 
				left++;		// 清除多余的 0
			s++;
		}
		c[topc++] = s + '0';
		if (a[topa] == '\0') break;
		tmp[right++] = a[topa++];
	}
	tmp[right] = '\0';
	strcpy(r, tmp + left);		// 生成余数
	if (r[0] == '\0') {r[0] = '0';  r[1] = '\0'; }
	c[topc] = '\0';
	clear_zero(c);			// 清除前导 0
	free(tmp);

	return 1;
}

int main(void)
{
	int i;

	while (1)
	{
		printf("输入除数: ");
		scanf("%s", a);
		printf("输入被除数: ");
		scanf("%s", b);

		// 检测被除数是不是 0
		for (i=0; b[i]!='\0'; i++)
			if (b[i] != '0') break;
		if (b[i] == '\0') return 0;

		if (div(a, b, c, r))		// 计算 a / b
			printf("商: %s\n余数: %s\n\n", c, r);
		else printf("内存不足\n\n");
	}
	return 0;
}
