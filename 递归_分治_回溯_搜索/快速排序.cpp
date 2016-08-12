#include <stdio.h>

int move(int *num, int left, int right)
{
	int key = num[left];

	while (left < right)
	{
		while (left < right && num[right] >= key) right--;
		num[left] = num[right];
		while (left < right && num[left] <= key) left++;
		num[right] = num[left];
	}
	num[left] = key;
	return left;
}

void quick_sort(int *num, int left, int right)
{
	int mid;

	if (left < right)		// 类似二叉树先序遍历
	{
		mid = move(num, left, right);
		quick_sort(num, left, mid);
		quick_sort(num, mid+1, right);
	}
}

int main(void)
{
	int n, i;
	int num[100];

	printf("输入一个 n, 及 n 个数字: ");
	scanf("%d", &n);
	for (i=0; i<n; i++) scanf("%d", num + i);
	quick_sort(num, 0, n - 1);
	printf("排序后: ");
	for (i=0; i<n; i++) printf("%d, ", num[i]);
	printf("\n");

	return 0;
}