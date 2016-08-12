#include <stdio.h>

// 快速排序的主要部分
// 函数完成 data[左区间] < data[mid] < data[右区间]
// 同时返回枢轴 left
int sort(int *data, int left, int right)
{
	int key;

	key = data[left];		// 选择 left 做枢轴
	while (left < right)
	{
		// 因为枢轴选在 left, 所以先从右边开始
		while (left < right && data[right] >= key) right--;
		data[left] = data[right];		// 此时 data[right] 是空闲的
		while (left < right && data[left] <= key) left++;
		data[right] = data[left];		// 此时 data[left] 是空闲的
	}
	// 此时 left == right, 所以下句代码的 data[left] 和 data[right] 是一样的
	data[left] = key;
	return left;		// 同理, 返回 right 也可
}

// 快速排序, 排序区间 [left..right]
// 区间分割, [left..mid-1] [mid] [mid + 1..right]
void quick_sort(int *data, int left, int right)
{
	int mid;		// 枢轴
	if (left < right)
	{
		mid = sort(data, left, right);
		quick_sort(data, left, mid - 1);
		quick_sort(data, mid + 1, right);
	}
}

int main(void)
{
	int i, n;
	int data[100];

	printf("请输入 n, 及 n 个元素: ");
	scanf("%d", &n);
	for (i=0; i<n; i++) scanf("%d", data + i);

	quick_sort(data, 0, n-1);
	printf("排序后: ");
	for (i=0; i<n; i++) printf("%d, ", data[i]);
	printf("\n\n");
	return 0;
}