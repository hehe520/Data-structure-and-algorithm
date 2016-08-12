#include <stdio.h>
#include <malloc.h>

// 归并排序的主要部分
// 合并 [left..mid] 与 [mid+1, right]
// 类似顺序表的合并
void merge(int *tmp, int *data, int left, int mid, int right)
{
	int pos = 0;		// 指示临时空间位置
	int rs = mid + 1;	// 右区间起点
	int ls = left;		// 左区间起点
	int i, j;

	// left, right 不能修改, 只能改变 ls, rs 指针
	while (ls <= mid && rs <= right)
	{
		if (data[ls] < data[rs])
			tmp[pos++] = data[ls++];
		else tmp[pos++] = data[rs++];
	}
	// 剩下的部分, 直接填入
	while (ls <= mid)   tmp[pos++] = data[ls++];
	while (rs <= right) tmp[pos++] = data[rs++];
	// 从 left 开始覆盖掉原来无序部分
	for (j=left, i=0; i<pos; i++, j++) data[j] = tmp[i];
}

// 2-路归并排序
// 划分区间 [left..mid], [mid+1..right]
// 想要分成 [left..mid-1], [mid..right] 也是可以的
void merge_sort(int *data, int left, int right)
{
	int mid;
	int tmp[101];	// 临时空间

	if (left < right)	//类似二叉树的后序遍历
	{
		mid = (left + right) / 2;
		merge_sort(data, left, mid);
		merge_sort(data, mid + 1, right);
		merge(tmp, data, left, mid, right);
		// 合并 [left..right]
	}
}

int main(void)
{
	int i, n;
	int data[100];

	printf("请输入 n, 及 n 个元素: ");
	scanf("%d", &n);
	for (i=0; i<n; i++) scanf("%d", data + i);

	merge_sort(data, 0, n-1);
	printf("排序后: ");
	for (i=0; i<n; i++) printf("%d, ", data[i]);
	printf("\n\n");
	return 0;
}