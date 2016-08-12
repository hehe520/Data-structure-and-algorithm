
#include <stdio.h>

#define NUM 500		// 最多 500 个数
#define MAX(a, b) ((a) > (b) ? (a) : (b))

struct 
{
	int left;
	int right;
	int max;
}tree[1000];		// 线段树

int num[NUM];		// 数组
int n;

int create(int L, int R, int pos)		// 创建树
{
	int mid;
	int a, b;

	if (L == R)
	{
		tree[pos].left = L;
		tree[pos].right = R;
		tree[pos].max = num[L];
		return num[L];
	}
	mid = (L + R) / 2;
	a = create(L, mid, 2 * pos);
	b = create(mid + 1, R, 2 * pos + 1);
	tree[pos].left = L;
	tree[pos].right = R;
	tree[pos].max = MAX(a, b);		// 回溯时获得最大值

	return tree[pos].max;
}

void DFS(int L, int R, int pos, int *m)
{
	int mid;

	if (tree[pos].left == L && tree[pos].right == R)
	{
		*m = MAX(*m, tree[pos].max);
		return ;
	}
	mid = (tree[pos].left + tree[pos].right) / 2;
	if (R <= mid)		// 左子树
	{
		DFS(L, R, 2 * pos, m);
	}
	else if (mid < L)		// 右子树
	{
		DFS(L, R, 2 * pos + 1, m);
	}
	else		// 横跨左右子树
	{
		DFS(L, mid, 2 * pos, m);
		DFS(mid + 1, R, 2 * pos + 1, m);
	}
}

int query(int L, int R)		// 查询最值
{
	int max = -0xFFFFFF;

	if (L < R)
		DFS(L, R, 1, &max);		// 线段树深搜
	else
		DFS(R, L, 1, &max);
	return max;
}

int main(void)
{
	int i;
	int a, b;

	printf("输入 n, 及 n 个数: ");
	scanf("%d", &n);
	for (i=1; i<=n; i++)		// 下标从 1 开始
		scanf("%d", num + i);
	create(1, n, 1);
	// 区间查询
	while (1)
	{
		scanf("%d %d", &a, &b);
		printf("最大值: %d\n", query(a, b));
	}
	return 0;
}
/*
10
3 2 4 5 6 8 1 2 9 7
*/