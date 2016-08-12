#include <stdio.h>

// 希尔排序其实就是多次插入排序
// 插入排序中的增量为常量 1, 这里为变量 dk
void shell_sort(int *data, int n)
{
	// 合适的增量序列
	int dk[] = {101, 70, 40, 20, 10, 7, 5, 3, 1};
	int pos = 0;
	int d, i, j;
	int tmp;

	while (dk[pos++] >= n);		// 确定增量起点

	for (d=pos-1; dk[d-1]!=1; d++)	// 缩小增量
	{
		for (i=dk[d]; i<n; i += dk[d])	// i 增加 dk[k]
		{
			tmp = data[i];		// 记录待插入结点
			if (data[i - dk[d]] > data[i])
			{
				for (j=i; j>0; j -= dk[d])
				{
					if (data[j - dk[d]] > tmp)
						data[j] = data[j - dk[d]];
					else break;
				}
				data[j] = tmp;
				// 上面的代码, 焦点在 i
			}
		}
	}
}

int main(void)
{
	int i, n;
	int data[100];

	printf("请输入 n, 及 n 个元素: ");
	scanf("%d", &n);
	for (i=0; i<n; i++) scanf("%d", data + i);

	shell_sort(data, n);
	printf("排序后: ");
	for (i=0; i<n; i++) printf("%d, ", data[i]);
	printf("\n\n");
	return 0;
}