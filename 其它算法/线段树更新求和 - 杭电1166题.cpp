
#include <stdio.h>

#define ADD 0
#define SUB 1

struct 
{
	int left;
	int right;
	int sum;
}tree[150000];		// 线段树

int num[50010];

int create(int L, int R, int pos)
{
	int a, b, mid;

	if (L == R)
	{
		tree[pos].left = L;
		tree[pos].right = R;
		tree[pos].sum = num[L];
		return num[L];
	}
	mid = (L + R) / 2;
	a = create(L, mid, 2 * pos);
	b = create(mid + 1, R, 2 * pos + 1);
	tree[pos].left = L;
	tree[pos].right = R;
	tree[pos].sum = a + b;

	return tree[pos].sum;
}

int node, inc, flag;	// 避免在递归中消耗过多的栈空间
// 修改一个结点的值
void update(int L, int R, int pos)
{
	if (tree[pos].left == node && tree[pos].right == node)
	{
		tree[pos].sum += inc;
		return;
	}
	int mid = (L + R) / 2;
	if (node <= mid)		// 左子树
	{
		update(L, mid, 2 * pos);
		tree[pos].sum += inc;
	}
	else	// 右子树
	{
		update(mid + 1, R, 2 * pos + 1);
		tree[pos].sum += inc;
	}
}

int total;
// 区间求和
void query(int L, int R, int pos)
{
	if (tree[pos].left == L && tree[pos].right == R)
	{
		total += tree[pos].sum;
		return ;
	}
	int mid = (tree[pos].left + tree[pos].right) / 2;
	if (R <= mid)		// 左子树
	{
		query(L, R, 2 * pos);
	}
	else if (mid < L)		// 右子树
	{
		query(L, R, 2 * pos + 1);
	}
	else		// 横跨左右子树
	{
		query(L, mid, 2 * pos);
		query(mid + 1, R, 2 * pos + 1);
	}
}

int main(void)
{
	int t, n, i, j;
	char cmd[10];

	scanf("%d", &t);
	for (j=0; j<t; j++)
	{
		printf("Case %d:\n", j + 1);
		scanf("%d", &n);
		for (i=1; i<=n; i++) scanf("%d", num + i);
		create(1, n, 1);
		while (1)		// 读入命令
		{
			scanf("%s", cmd);
			if (cmd[0] != 'E')
			{
				scanf("%d %d", &node, &inc);
				if (cmd[0] == 'A') 
					update(1, n, 1);
				else if (cmd[0] == 'S') 
				{
					inc = -inc;
					update(1, n, 1);
				}
				else if (cmd[0] == 'Q')
				{
					total = 0;
					query(node, inc, 1);
					printf("%d\n", total);
				}
			}
			else break;
		}
	}
	return 0;
}
/*
2
10
1 2 3 4 5 6 7 8 9 10
Query 1 3
Add 3 6
Query 2 7
Sub 10 2
Add 6 3
Query 3 10
End
10
1 2 3 4 5 6 7 8 9 10
Query 1 3
Add 3 6
Query 2 7
Sub 10 2
Add 6 3
Query 3 10
End
*/