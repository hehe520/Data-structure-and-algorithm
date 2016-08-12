#include <stdio.h>
#include <malloc.h>
#include <limits.h>

#define INF INT_MAX

typedef struct		// 动态二维数组
{
	int **m;
	int x;
	int y;
}suzhu;

typedef struct		// 维度 (x * y)
{
	int x;
	int y;
}matrix;

void create(suzhu *n, int x, int y)
{
	int i, j;

	n->x = x;   n->y = y;
	n->m = (int **)malloc(x * sizeof(int *));
	for (i=0; i<x; i++)
	{
		n->m[i] = (int *)malloc(y * sizeof(int));
		for (j=0; j<y; j++) n->m[i][j] = 0;		// 初始化
	}
}

void destroy(suzhu *n)
{
	int i;

	if (n->m == NULL) return ;
	for (i=0; i<n->x; i++)
		free(n->m[i]);
	free(n->m);
	n->m = NULL;
}

void show(suzhu *n)
{
	int i, j;

	for (i=0; i<n->x; i++)
	{
		for (j=0; j<n->y; j++)
		{
			if (i > j) printf("%8c,", '-');
			else printf("%8d,", n->m[i][j]);
		}
		printf("\n");
	}
}

int ok(matrix *m, int n)		// 矩阵可乘检查
{
	int i;

	for (i=1; i<n; i++)
		if (m[i-1].y != m[i].x) return 0;
	return 1;
}

// 矩阵连乘, m 记录矩阵维度, c 记录相乘次数, p 记录位置
void dp(matrix *m, int n, suzhu *c, suzhu *p)
{
	int i, j, k;
	int tmp;
	int end;

	// 自己和自己相乘是 0 次
	for (i=0; i<n; i++) p->m[i][i] = c->m[i][i] = 0;
	for (i=1; i<n; i++)		// i 控制次数
	{
		for (j=0; j<n-i; j++)
		{
			end = j + i;			// 锁定末尾 [j .. end]
			c->m[j][end] = INF;		// 两张表的初始化
			p->m[j][end] = j + 1;
			for (k=j; k<end; k++)		// k 在 j..end 尝试切断
			{
				tmp = c->m[j][k] + c->m[k+1][end] 
					+ m[j].x * m[k+1].x * m[end].y;
				if (tmp < c->m[j][end])
				{
					c->m[j][end] = tmp;
					p->m[j][end] = k + 1;
				}
			}
		}
	}
}

int main(void)
{
	suzhu count, pos;
	int i, n;
	matrix m[50];		// 最多 50 个矩阵相乘

	printf("输入 n, 及 n 个矩阵的维度: ");
	scanf("%d", &n);
	for (i=0; i<n; i++)
		scanf("%d %d", &(m[i].x), &(m[i].y));

	create(&count, n, n);		// 记录相乘次数
	create(&pos, n, n);		// 记录加括号的位置
	if (!ok(m, n)) { printf("矩阵不可乘!\n");  return 0;}

	dp(m, n, &count, &pos);		// DP

	printf("\n最优连乘次数表: \n");
	show(&count);
	printf("\n加括号位置: \n");
	show(&pos);

	destroy(&count);
	destroy(&pos);
	return 0;
}
