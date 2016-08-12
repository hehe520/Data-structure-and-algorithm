
// 利用归并排序求逆序对, O(n log n)

#include <stdio.h>
#include <malloc.h>

int *tmp;	// 归并的辅助空间
int count;	// 逆序对数量

// 合并, 参数不能改
void merge(int *num, int ls, int le, int rs, int re)
{
	int lc = ls;	// 指针设定在起点
	int rc = rs;
	int top = 0;
	int i;

	// 指针没有到头
	while (lc <= le && rc <= re)
	{
		if (num[lc] <= num[rc])
			tmp[top++] = num[lc++];
		else 
		{
			tmp[top++] = num[rc++];
			// 此时右区间的的数比左区间剩下的都要大
			// 那么 num[rc] 与剩下 num[lc..le] 构成逆序对
			// 可以将他们全部输出, 这里只统计数量
			count += le - lc + 1;
		}
	}
	while (lc <= le) tmp[top++] = num[lc++];
	while (rc <= re) tmp[top++] = num[rc++];
	// 将 tmp 的结果写回去
	for (i=ls, top=0; i<=re; i++)
		num[i] = tmp[top++];
}

void dfs(int *num, int left, int right)
{
	int mid;

	if (left < right)
	{
		mid = (left + right) / 2;
		dfs(num, left, mid);
		dfs(num, mid + 1, right);
		// 合并 [left, mid], [mid+1, right]
		merge(num, left, mid, mid+1, right);
	}
}

// 归并排序求逆序对
void g_sort(int *num, int n)
{
	// 归并的辅助空间
	tmp = (int *)malloc(1000 * sizeof(int));
	count = 0;
	dfs(num, 0, n-1);
	free(tmp);
}

int main(void)
{
	int n, i;
	int num[1000];

	while (1)
	{
		printf("输入 n, 及 n 个数字: ");
		scanf("%d", &n);
		if (n <= 0) break;
		for (i=0; i<n; i++)
			scanf("%d", num + i);
		g_sort(num, n);			// 归并排序
		for (i=0; i<n; i++)
			printf("%d, ", num[i]);
		printf("逆序对: %d\n\n", count);
	}
	return 0;
}