#include <stdio.h>

// 桶排序, 桶排序不支持负数, 

#define MAX_SIZE 1000	// 最大数据

// 类似地图标记法
void tong_sort(int *data, int n)
{
	unsigned short int map[MAX_SIZE];
	int i, j;

	// map[i] 记录 i 出现了多少次
	for (i=0; i<MAX_SIZE; i++) map[i] = 0;
	for (i=0; i<n; i++) map[ data[i] ]++;
	for (j=i=0; i<MAX_SIZE; i++)
	{
		while (map[i]--) data[j++] = i;
	}	// 次数--, data[j] 记录元素
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