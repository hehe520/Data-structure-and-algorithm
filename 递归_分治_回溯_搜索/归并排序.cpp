#include <stdio.h>

void merge(int *num, int ls, int le, int rs, int re, int *tmp)
{
	int left = ls;
	int right = rs;
	int top = 0, i;

	while (left <= le && right <= re)
	{
		if (num[left] < num[right])
			tmp[top++] = num[left++];
		else tmp[top++] = num[right++];
	}
	while (left <= le) tmp[top++] = num[left++];
	while (left <= le) tmp[top++] = num[left++];
	for (i=0; i<top; i++) num[ls++] = tmp[i];
}

void merge_sort(int *num, int left, int right)
{
	int mid;
	static tmp[100];

	if (left < right)		// 分割区间
	{
		mid = (left + right) / 2;
		merge_sort(num, left, mid);
		merge_sort(num, mid+1, right);
		merge(num, left, mid, mid+1, right, tmp);
	}
}

int main(void)
{
	int n, i;
	int num[100];

	printf("输入一个 n, 及 n 个数字: ");
	scanf("%d", &n);
	for (i=0; i<n; i++) scanf("%d", num + i);
	merge_sort(num, 0, n - 1);
	printf("排序后: ");
	for (i=0; i<n; i++) printf("%d, ", num[i]);
	printf("\n");

	return 0;
}
