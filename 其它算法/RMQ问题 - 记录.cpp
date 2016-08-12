
// O(n^2) 计算每个查询对, 用 RMQ[][] 保存结果, O(1) 回答查询

#include <stdio.h>

#define NUM 300

int RMQ[NUM][NUM];
int num[NUM];
int n;

void initialize(void)
{
	int i, j;
	int max;

	for (i=0; i<n; i++)
	{
		max = -0xFFFFFF;
		for (j=i; j<n; j++)
		{
			if (num[j] > max) max = num[j];
			RMQ[i][j] = max;
			RMQ[j][i] = max;
		}
	}
}

int main(void)
{
	int a, b;
	int i;

	printf("输入 n, 及 n 个数: \n");
	scanf("%d", &n);
	for (i=0; i<n; i++)
		scanf("%d", num + i);
	initialize();
	// 输入查询区间, 下标从 0 开始
	while (1)
	{
		scanf("%d %d", &a, &b);
		printf("最大值: %d\n", RMQ[a][b]);
	}
	return 0;
}
/*		一组测试数据
10
3 2 4 5 6 8 1 2 9 7
*/