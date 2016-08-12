
// 求 n 的阶乘, 

#include <stdio.h>

int num[1000000];		// 每个元素保存 5 位整数, 所以精度为 10^5000000
int top;

void calc(int n)
{
	int i, j;
	int tmp, cf = 0;

	top = 0;
	num[top++] = 1;
	if (n == 0 || n == 1) return ;
	for (i=1; i<=n; i++)
	{
		for (j=0; j<top; j++)
		{
			tmp = num[j] * i + cf;
			num[j] = tmp % 100000;
			cf = tmp / 100000;
		}
		// 如果有进位, 开多一个 num[] 元素
		if (cf != 0)
		{
			num[top++] = cf;
			cf = 0;
		}
	}
}

void show(void)
{
	int i;

	printf("%d", num[--top]);
	for (i=top-1; i>=0; i--)
	{
		printf("%05d", num[i]);
	}
	printf("\n");
}

int main(void)
{
	int n;

	while (1)
	{
		printf("输入 n: ");
		scanf("%d", &n);
		calc(n);
		show();
	}
	return 0;
}