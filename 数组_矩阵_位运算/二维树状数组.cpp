
// 二维树状数组, 和一维的进行类比

#include <stdio.h>
#include <string.h>

#define MAX 100		// 矩阵大小100 * 100
#define LOWBIT(x) (x & (-x))


// 更新一个结点, 从底层走到根
void update(int (*c)[MAX+1], int x, int y, int data)
{
	int i, j;

	for (i=x; i<MAX; i += LOWBIT(i))
		for (j=y; j<MAX; j += LOWBIT(j))
			c[i][j] += data;
}

// 求 (1, 1) 到 (x, y) 矩阵的和, 
int sum(int (*c)[MAX+1], int x, int y)
{
	int i, j;
	int sum = 0;

	for (i=x; i>0; i -= LOWBIT(i))
		for (j=y; j>0; j -= LOWBIT(j))
			sum += c[i][j];
	return sum;
}

int main(void)
{
	int n, m;
	int i, j;
	int c[MAX+1][MAX+1];
	int data;

	memset(c, 0, sizeof(c));
	printf("请输入一个矩阵: ");
	scanf("%d %d", &n, &m);
	for (i=1; i<=n; i++)	
	{
		for (j=1; j<=m; j++)
		{
			scanf("%d", &data);
			update(c, i, j, data);
		}
	}
	while (1)
	{
		printf("\n矩阵求和, 终点坐标: ");
		scanf("%d %d", &n, &m);
		printf("\n矩阵和为: %d\n", sum(c, n, m));
	}
	return 0;
}