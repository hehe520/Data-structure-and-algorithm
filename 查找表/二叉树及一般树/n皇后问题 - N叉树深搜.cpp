#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// n皇后问题（深度搜索）
// 其实就是深度为 n 的 n 叉满树的遍历
// 遍历方法类似孩子兄弟树的遍历
// 如果能搜索到最底层就表示找到合法的解

static int num = 0;		// 统计合法个数

// 输出地图
void show(char (*map)[20], int n, int num)
{
	int i, j;

	printf("第 %d 个解:\n", num);
	for (i=0; i<n; i++)
	{
		for (j=0; j<n; j++)
			printf("%2c", map[i][j]);
		printf("\n");
	}
	printf("\n");
}

// 判断 x行 i列，能不能走
short int OK(char (*map)[20], int n, int x, int i)
{
	int j;
	int tx = x, ti = i;	// 保存多一份x,i

	// 检测同一列，行就不用检测了，因为就是要放当前行
	for (j=0; j<x; j++)
	{
		if (map[j][i] == '*')  return 0;
	}
	// 检测左对角
	while ((x+1) && (i+1))
	{
		if (map[x][i] == '*')  return 0;
		x--;   i--;
	}
	// 检测右对角
	while ((tx+1) && (ti+1))
	{
		if (map[tx][ti] == '*')  return 0;
		tx--;   ti++;
	}
	return 1;
}

// 深度搜索
// 搜索到第 n 层的时候，显然找到解了
int DFS(char (*map)[20], int n, int x)
{
	int i;

	// 如果棋盘放满了(找到解)，进行输出
	if (n == x)  show(map, n, ++num);
	else 
	{
		// 如果没满，继续深度搜索
		for (i=0; i<n; i++)   // 探测当前x行，类似树的孩子兄弟法的遍历
		{
			if (OK(map, n, x, i))   // 如果可以放
			{
				map[x][i] = '*';	// 传递时，标记
				DFS(map, n, x+1);	// 继续递归搜索
				map[x][i] = '.';	// 回溯时，擦图
			}
		}
	}
	return num;
}

// N皇后, 返回搜索到的合法棋盘数
int Nqueen(int n)
{
	char map[20][20];

	memset(map, '.', sizeof(map));	// 初始化地图
	num = 0;
	return DFS(map, n, 0);		// 开始深搜
}

int main(void)
{
	int n;
	int num;
	
	while (1)
	{
		printf("请输入皇后数：");
		scanf("%d", &n);
		system("cls");
		
		num = Nqueen(n);  // 搜索并输出
		
		printf("共找到 %d 个合法的棋盘\n\n", num);
	}
	return 0;
}
