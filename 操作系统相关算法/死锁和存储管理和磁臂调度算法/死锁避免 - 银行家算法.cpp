
// 死锁的避免之银行家算法
// 有 n 个进程, m 个不同的资源
// 每个进程占用 m 个资源中的一个或多个

#include <stdio.h>
#include <string.h>

#define NUM 50

int n, m;
int xian[NUM];			// 空闲资源
int you[NUM][NUM];		// 已有资源
int xu[NUM][NUM];		// 仍然需求
// xian[a] = b;      表示第 a 号资源的空闲数是 b
// you[a][b] = c;    表示第 a 个进程, 占用 b 号资源 c 个
// xu[a][b] = c;     表示第 a 个进程, 仍需 b 号资源 c 个

// 安全返回 1, 不安全返回 0
int juage()
{
	int i, j, k;
	int used[NUM];
	int flag;

	for (k=0; k<n; k++) used[k] = 0;

	// n 个进程, 需要分配 n 次
	for (k=0; k<n; k++)
	{
		for (i=0; i<n; i++)		// 挑一个 i
		{
			if (used[i]) continue;
			flag = 1;
			for (j=0; j<m; j++)		// 匹配空闲
			{
				if (xian[j] - xu[i][j] < 0)
				{
					flag = 0;
					break;
				}
			}
			if (flag) break;
		}
		if (!flag) return 0;
		used[i] = 1;
		for (j=0; j<m; j++)		// 释放 i 的资源
			xian[j] += you[i][j];
	}
	return 1;
}

int main(void)
{
	int i, j;

	memset(you, 0, sizeof(you));
	memset(xu, 0, sizeof(xu));
	memset(xian, 0, sizeof(xian));
	printf("请输入进程数: ");
	scanf("%d", &n);
	printf("请输入资源种类个数: ");
	scanf("%d", &m);
	printf("输入已有资源矩阵: \n");
	for (i=0; i<n; i++)
		for (j=0; j<m; j++)
			scanf("%d", you[i] + j);
	printf("输入仍然需求矩阵: \n");
	for (i=0; i<n; i++)
		for (j=0; j<m; j++)
			scanf("%d", xu[i] + j);
	printf("输入空闲资源向量: ");
	while (1)
	{
		for (i=0; i<m; i++)
			scanf("%d", xian + i);
		if (juage()) printf("安全状态\n");
		else printf("不安全状态\n");
	}
	return 0;
}
/*		两组测试数据
5 4
3 0 1 1
0 1 0 0
1 1 1 0
1 1 0 1
0 0 0 0

1 1 0 0
0 1 1 2
3 1 0 0
0 0 1 0
2 1 1 0

0 0 1 0
1 0 0 0
*/