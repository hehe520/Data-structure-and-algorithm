#include <stdio.h>

int count = 1;

inline void swap(int *a, int *b)
{
	int tmp = *a;
	*a = *b;   *b = tmp;
}

void show_p(int num[], int s, int all)
{
	int i;

	if (s == all-1) 
	{
		printf("第 %d 个排列: ", count++);
		for (i=0; i<all; i++)
			printf("%d, ", num[i]);
		printf("\n");
	}
	else 
	{
		for (i=s; i<all; i++)
		{
			swap(num + s, num + i);
			show_p(num, s+1, all);		// 对 s + 1 继续排列
			swap(num + s, num + i);		// 回溯改回原来的
		}
	}
}

int main(void)
{
	int num[200];
	int n, i;

	printf("输入 n, 及 n  个元素: ");
	scanf("%d", &n);
	for (i=0; i<n; i++)
		scanf("%d", num+i);
	show_p(num, 0, n);

	return 0;
}
