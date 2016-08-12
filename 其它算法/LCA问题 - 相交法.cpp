
// 在一棵树上求两个结点的最近公共祖先
// 从两个结点往根走, 最先两次被访问的结点就是最近的祖先

#include <stdio.h>

int tree[1000];		// 二叉树静态存储
int add[5000];		// 结点地址
int num[500];		// 保存输入数字
int top;
int n;				// 结点数
int used[500];		// 访问次数

// num 保存数组, pos 二叉树下标, f 数组下标
void create(int pos)
{
	if (num[top] == -1)
	{
		top++;
		return ;
	}
	tree[pos] = num[top++];
	add[num[top-1]] = pos;
	create(2 * pos);
	create(2 * pos + 1);
}

int LCA(int a, int b)
{
	int i, pos;

	for (i=0; i<n; i++) used[i] = 0;
	pos = add[a];
	while (pos != 0)		// 从 a 往根走
	{
		used[pos]++;
		pos /= 2;
	}
	pos = add[b];
	while (pos != 0)		// 从 b 往根走
	{
		if (used[pos] == 1)
		{
			i = tree[pos];
			break;
		}
		pos /= 2;
	}
	return i;
}

int main(void)
{
	int a, b;

	// 类似二叉树的输入方式, -1 表示空树, -2 表示输入结束
	// 测试数据: 0 1 3 -1 -1 4 5 -1 -1 6 -1 -1 2 7 -1 -1 8 -1 -1 -2
	printf("输入一棵树: ");
	top = 0;
	while (1)
	{
		scanf("%d", num + top);
		if (num[top] == -2) break;
		top++;
	}
	n = top;
	top = 0;
	create(1);
	printf("LCA 查询:\n");
	while (1)
	{
		scanf("%d %d", &a, &b);
		printf("最近公共祖先: %d\n", LCA(a, b));
	}
	return 0;
}