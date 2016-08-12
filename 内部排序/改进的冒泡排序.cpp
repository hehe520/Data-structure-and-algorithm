#include <stdio.h>

// 冒泡排序, 大数上浮, 小数下沉
void msort(int *data, int n)
{
	int tmp;
	int i, j;
	int last;		// 记录最后交换次数

	// n 个元素, 最多拍 n-1 趟
	for (i=0; i<n-1; i++)
	{
		last = 0;
		for (j=last; j<n-1; j++)
		{
			if (data[j] > data[j+1])
			{
				tmp = data[j];
				data[j] = data[j+1];
				data[j+1] = tmp;
				last = j;		// 记录
			}
		}
		if (last == 0) break;
	}
}

int main(void)
{
	int i, n;
	int data[100];

	printf("请输入 n, 及 n 个元素: ");
	scanf("%d", &n);
	for (i=0; i<n; i++) scanf("%d", data + i);

	msort(data, n);
	printf("排序后: ");
	for (i=0; i<n; i++) printf("%d, ", data[i]);
	printf("\n\n");
	return 0;
}