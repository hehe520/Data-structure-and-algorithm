#include <stdio.h>
#include <windows.h>

#define SIZE 200
inline void copy(int *d, int *s, int n);	// 数组复制

int c, n;			// 最大容量 c, 总数 n
int weight[SIZE];	// 重量
int value[SIZE];	// 价值
int used[SIZE];		// 记录拿了哪些物品
int best_value = -0XFFFF;

int v = 0;			// 递归时, 临时的变量
int w = 0;
int u[SIZE];
// 为了减少消耗递归栈容量, 把一些变量设为全局变量


// 回溯法解决0-1背包问题, 搜索状态树
// 对于第 i 件物品可以选择拿或不拿, 左子树是拿, 右子树不拿
// i 表示搜索到了第 i 件物品, 
void DFS(int i)
{
	if (i == n)		// 到了叶子, 算总账
	{
		if (v > best_value) 
		{
			best_value = v;		// 记录最优价值
			copy(used, u, n);	// 记录拿了哪些
		}
	}
	else 
	{
		// 判断能不能进入左子树, 重量不能超过背包总重
		if (w + weight[i] <= c)
		{
			w += weight[i];		// 拿了
			v += value[i];
			u[i] = 1;
			DFS(i + 1);			// 搜索下一件物品
			w -= weight[i];		// 回溯时, 恢复现场
			v -= value[i];		// 不影响下一次搜索
			u[i] = 0;
		}
		DFS(i + 1);		// 右子树是不拿的, 直接进入即可
	}
	return ;
}

int main(void)
{
	int i;

	printf("输入背包最大容量及物品总数: ");
	scanf("%d %d", &c, &n);
	printf("输入 %d 件物品: (重量 价值)\n", n);
	for (i=0; i<n; i++)
		scanf("%d %d", weight + i, value + i);
	DFS(0);			// 搜索
	printf("\n选了: ");
	// 同时输出被选物品
	for (i=0; i<n; i++)
	{
		if (used[i]) printf("(%d, %d), ", weight[i], value[i]);
	}
	printf("\n最大价值: %d\n", best_value);
	return 0;
}

inline void copy(int *d, int *s, int n)
{
	int i;
	for (i=0; i<n; i++) d[i] = s[i];
}