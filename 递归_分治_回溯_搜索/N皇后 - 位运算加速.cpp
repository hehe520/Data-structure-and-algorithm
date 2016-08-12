
#include <stdio.h>

int all;		// 前 n 个位都是 1
int sum;

void DFS(int cur, int ld, int rd)
{
	int p, pos;

	if (cur == all)
	{
		sum++;
		return ;
	}
	pos = all & ~(cur | ld | rd);
	// 获得所有不冲突的位置
	while (pos)
	{
		p = pos & -pos;		// 获得最右边不冲突位置
		pos -= p;			// 去掉该位置
		DFS(cur + p, (ld + p) << 1, (rd + p) >> 1);
	}
}

int main(void)
{
	int n;

	while (1)
	{
		// 输入 n 不要超过 20, n 表示 n * n 的棋盘
		scanf("%d", &n);
		all = (1 << n) - 1;		// 前 n 位置为 1
		sum = 0;
		DFS(0, 0, 0);
		printf("%d\n", sum);		// 输出解的个数
	}
	return 0;
}