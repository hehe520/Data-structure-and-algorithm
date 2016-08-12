#include <stdio.h>
#include <string.h>

// 树状数组对于区间求和很有效, 能在 O(log n) 内完成

// 求 n 的因子中, 最大的 2^k
inline int lowbit(int t)
{
	return t & (-t);
}

// 改了a[i], 同时也要修改所有覆盖了 a[i] 的 c[] 结点
void add(int *c, int pos, int data, int n)
{
	while (pos <= n)
	{
		c[pos] += data;
		pos += lowbit(pos);		// 向双亲结点走
	}
}

// 求前 n 项和
int sum(int *c, int n) 
{
	int sum = 0;

	while (n > 0)
	{
		sum += c[n];
		n -= lowbit(n);
	}
	return sum;
}

// 求区间 [a,b] 的和
int sum_a_b(int *c, int a, int b)
{
	return sum(c, b) - sum(c, a-1);
}

int main(void)
{
	int c[100];		// 树状数组
	int i, n;
	int data;
	int a, b;
	
	memset(c, 0, sizeof(c));
	printf("建立数组, 输入 n, 及 n 个元素: ");
	scanf("%d", &n);
	for (i=1; i<=n; i++)
	{
		scanf("%d", &data);
		add(c, i, data, n);		// 同时修改 c 数组 
	}
	printf("请输入一个区间求和: ");
	scanf("%d %d", &a, &b);
	printf("\n和为: %d\n", sum_a_b(c, a, b));

	return 0;
}