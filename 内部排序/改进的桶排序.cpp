#include <stdio.h>
#include <malloc.h>

// 改进的桶排序, 支持负数

#define SIZE 1000000	// 最大辅助空间

// 算法: 找到一个最小的, 最大的, 计算差值, 开临时数组
// 将原数组整体前移, 最小的数在 0 位置
// 比如 最小是 -1, 那就把 -1 放在 0 位置,
void tong_sort(int *data, int n)
{
	int min = 0xFFFFFFF;
	int max = -0xFFFFFFF;
	int j, i, size;
	short int *map;		// 临时空间

	for (i=0; i<n; i++)		// 先找出最值
	{
		if (data[i] > max) max = data[i];
		if (data[i] < min) min = data[i];
	}
	size = max-min;
	if (size > SIZE) return ;	// 防止太大了
	map = (short int *)malloc((size+2) * sizeof(short int));

	for (i=0; i<=size; i++) map[i] = -1;		// 初始
	for (i=0; i<n; i++) 
	{
		map[ data[i] - min ]++;		// 标记
	}
	for (j=i=0; i<=size; i++)		// 回收
	{
		while (map[i] != -1) 
		{
			data[j++] = i + min;	// 加回差值 min
			map[i]--;
		}
	}
	free(map);
}

int main(void)
{
	int i, n;
	int data[100];

	printf("请输入 n, 及 n 个元素: ");
	scanf("%d", &n);
	for (i=0; i<n; i++) scanf("%d", data + i);

	tong_sort(data, n);
	printf("排序后: ");
	for (i=0; i<n; i++) printf("%d, ", data[i]);
	printf("\n\n");
	return 0;
}