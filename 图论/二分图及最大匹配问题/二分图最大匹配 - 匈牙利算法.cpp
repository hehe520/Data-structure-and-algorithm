
// 二分图的最大匹配数 - 匈牙利算法
// 不断的查找交错增广路增加匹配数

#include <stdio.h>

#define NUM 70
#define NO -1

int used[NUM];
int link[NUM];
// link[b] = a;		表示 b 已经匹配到 a 了
// a, b 分别属于两个不同的集合, 只需记录其中一个集合的匹配情况即可
// 因为如果 b 匹配了 a 了, b 就不可能再匹配 c 了
// 否则 b 一个点, 匹配了到 a, c 的两条边, 就不是二分匹配了
// 所以 link[] 只记录 b 集合的匹配情况


// 查找一条交错增广路, 即首尾是未匹配的点, 其余匹配未匹配交错
// 如: [未匹配, 匹配, 未匹配, 匹配, ,,, 未匹配]
int findpath(int (*m)[NUM], int cur)
{
	int i;

	// 遍历 cur 能到的边, 进行匹配
	for (i=0; i<NUM; i++)
	{
		if (!used[i] && m[cur][i])		// 类似深搜
		{
			used[i] = 1;
			// 如果 i 点没有匹配过, 即 cur->i 是没匹配过的, 那直接匹配
			// 如果 i 点已经匹配过了, 则尝试从 linkk[i] 点匹配
			if (link[i] == -1 || findpath(m, link[i]))
			{
				link[i] = cur;		// 回溯时更新匹配
				return 1;
			}
		}
	}
	return 0;
}

// 进行匹配
int pipei(int (*m)[NUM], int n)
{
	int count = 0;		// 找到的增广路个数
	int i, j;

	for (i=0; i<NUM; i++) link[i] = NO;
	for (i=0; i<n; i++)		// 遍历所有点
	{
		for (j=0; j<NUM; j++) used[j] = 0;
		if (findpath(m, i)) count++;
	}
		// 一个增广路的翻转, 增加一个匹配
	return count;
}

int main(void)
{
	int i, j;
	int n;
	int m[NUM][NUM];	// m 当成静态邻接表用
	int k;
	int a, b;

	printf("输入 A 集合点的个数:");
	scanf("%d", &n);
	for (i=0; i<n; i++)
		for (j=0; j<NUM; j++) m[i][j] = 0;
	while (1)
	{
		// 输入时, 约定 a 属于 a 集, b 属于 b 集
		scanf("%d %d", &a, &b);
		if (a == 0 && b == 0) break;
		m[a][b] = 1;
	}
	k = pipei(m, n);		// 最大匹配
	printf("\n最大匹配数: %d\n匹配的边: ", k);
	for (i=0; i<NUM; i++)
	{
		if (link[i] != NO)
			printf("(%d, %d), ", link[i], i);
	}
	putchar('\n');
	return 0;
}
