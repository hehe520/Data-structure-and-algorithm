#include <stdio.h>

// 顺序插入排序, [0..k][k+1..n]
// [0..k] 有序,  [k+1..n] 无序
// 每次从 k + 1 选一个插到 [0..k] 中的合适位置
void insert_sort(int *data, int n)
{
	int i, j, tmp;

	for (i=1; i<n; i++)		// i 属于 [k+1..n]
	{
		if (data[i-1] > data[i])
		{
			tmp = data[i];		// 腾出空位, 减少赋值
			for (j=i; j>0; j--)		// 找个合适位置
				if (data[j-1] > tmp)
					data[j] = data[j-1];
				else break;
			data[j] = tmp;		// 插入
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

	insert_sort(data, n);
	printf("排序后: ");
	for (i=0; i<n; i++) printf("%d, ", data[i]);
	printf("\n\n");
	return 0;
}
