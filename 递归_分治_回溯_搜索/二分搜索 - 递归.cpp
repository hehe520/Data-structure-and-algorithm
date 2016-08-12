#include <stdio.h>

// 递归的二分搜索, 速度显然要比非递归的慢
int bin_search_s(int *a, int left, int right, int data)
{
	int mid;

	if (left <= right)
	{
		mid = (left + right) / 2;
		if (a[mid] == data) return 1;
		else if (data < a[mid])
		{
			if (bin_search_s(a, left, mid-1, data)) 
				return 1;
		}
		else	// 递归搜索左右子树, 搜到就返回成功
		{
			if (bin_search_s(a, mid+1, right, data))
				return 1;
		}
	}
	return 0;
}

int main(void)
{
	int n, i;
	int a[100];
	int data;

	printf("请输入一个升序数组: ");
	scanf("%d", &n);
	for (i=0; i<n; i++)
		scanf("%d", a + i);

	while (1)
	{
		printf("\n请输入一个要查找的数: ");
		scanf("%d", &data);
		if (bin_search_s(a, 0, n-1, data))
			printf("找到!\n");
		else printf("未找到!\n");
	}
	return 0;
}