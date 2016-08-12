#include <stdio.h>

#define SIZE 300

// 递归输出, 如果路径过长, 则非递归的方法
void show(int *num, int *pre, int pos)
{
	if (pos != -1) 
	{
		show(num, pre, pre[pos]);
		printf("%d, ", num[pos]);
	}
}

// 每次从 d[] 数组中选一个最长且不超过自己的路径
// 在该长度上加一作为自己的最大长度
// d[i] = max{d[k] + 1}, k < i, a[k] <= a[i]
// 最后遍历 d[] 数组找一个最大的长度返回
int dp(int *num, int n)
{
	int d[SIZE];	// 记录目前最大长度
	int i, j;
	int max, len = -0xFFFFF;
	int pos;			// len 记录最大长度, pos 记录 len 的位置
	int pre[SIZE];		// 记录从哪个数字而来, j 的前驱是 pre[j]

	for (i=0; i<n; i++) d[i] = 1;	// 初始化成 1
	for (i=0; i<n; i++) pre[i] = -1;	// 初始化成 -1, 表示无前驱
	for (i=1; i<n; i++)		// 求 d[i]
	{
		max = 0;
		for (j=i-1; j>=0; j--)	// 去前面找个最大长度, 找不到的话 max = 0
		{
			if (num[j] <= num[i] && d[j] >= max)
			{
				max = d[j];
				pre[i] = j;		// 记录来自哪个数字
			}
		}
		d[i] = max + 1;		// 再加一, 为目前的最长长度
		if (d[i] >= len)
		{
			len = d[i];		// 更新最大长度
			pos = i;		// 记录最大位置
		}
	}
	printf("\n上升子序列: ");
	show(num, pre, pos);
	return len;
}

int main(void)
{
	int i, n;
	int num[SIZE];
	int len;

	printf("输入 n, 及 n 个数字: ");
	scanf("%d", &n);
	for (i=0; i<n; i++) scanf("%d", num + i);
	len = dp(num, n);
	printf("\n最大上升长度 %d\n", len);

	return 0;
}
