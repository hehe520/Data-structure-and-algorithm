
// 求最大子矩阵和
// 由求最大子段和推广, 最大子段和可看成是 1 * n 的矩阵
// m * n 的矩阵先将其压缩成 1 * n 的矩阵, 再用子段和的方法求出
// 压缩方法就是把 m 行相加
// 如         9   2   -6   2
//           -4   1   -4   1
// 两行相加   5   3   -10  3     对此求最大和
// 枚举 m * n 中, m 行的压缩情况

#include <stdio.h>
#include <malloc.h>

#define MAX 200

// 求最大子段和
int max_sum(int *a, int n, int *s, int *e)
{
	int max = 0;
	int i;
	int sum = -0XFFFFF;

	*s = *e = 0;
	for (i=0; i<n; i++)
	{
		if (max > 0) max += a[i];
		else { max = a[i];   *s = i; }		// 同时更新起点 s
		if (max > sum) { sum = max;  *e = i; }
	}
	return sum;
}

// 求最大子矩阵和, (a, b) 到 (c, d) 的子矩阵
int dp(int (*m)[MAX], int x, int y, 
	   int *a, int *b, int *c, int *d)
{
	int i, j, k;
	int *buf;
	int sum = -0XFFFFF;
	int tmp;
	int s, e;	// 子段和的范围 [s..e]

	*a = *b = *c = *d = 0;
	buf = (int *)malloc(y * sizeof(int));
	for (i=0; i<x; i++)		// 枚举起始行
	{
		for (j=0; j<y; j++) buf[j] = 0;
		for (j=i; j<x; j++)		// 从起始到最后一行
		{
			// 不断累加第 j 行
			for (k=0; k<y; k++) buf[k] += m[j][k];
			tmp = max_sum(buf, y, &s, &e);
			if (tmp > sum) 
			{
				sum = tmp;
				*a = i;   *c = j;	// 记录 i 行到 j 行
				*b = s;   *d = e;	// 记录 s 列到 e 列
			}
		}
	}
	free(buf);
	return sum;
}

int main(void)
{
	int m[MAX][MAX];
	int sum;
	int i, j;
	int x, y;		// 矩阵大小 (x * y)
	int a, b, c, d;		// 子矩阵 (a, b) 到 (c, d)

	printf("输入一个矩阵: ");
	scanf("%d %d", &x, &y);
	for (i=0; i<x; i++)
		for (j=0; j<y; j++) scanf("%d", &(m[i][j]));
	sum = dp(m, x, y, &a, &b, &c, &d);
	printf("\n子矩阵最大和: %d\n子矩阵:\n", sum);
	for (i=a; i<=c; i++)
	{
		for (j=b; j<=d; j++) printf("%5d", m[i][j]);
		printf("\n");
	}
	printf("\n");
	return 0;
}
