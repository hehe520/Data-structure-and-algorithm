
// 旅行售货员问题, 一个无向图中, 每条边都有权值
// 每个点都要经过, 找一条最小权值的回路

#include <stdio.h>

#define INF 0xFFFFFF
#define NUM 30			// 最大顶点数
int map[NUM][NUM];		// 邻接矩阵
int used[NUM];			// 访问标记
int pre[NUM];			// 路径信息

int n;					// 顶点数 < NUM
int value;				// 最大价值
int tmp_pre[NUM];		// 临时变量

// 回溯法, 深搜, 起点默认是 0, v 是当前价值, f 是深度
void DFS(int s, int v, int f)
{
	int i;
	if (f == n-1)		// 到了叶子算总账
	{
		if (v + map[s][0] < value)		// 形成回路, 加上 s->0 的权值
		{
			value = v + map[s][0];		// 记录最小值, 和路径
			for (i=0; i<n; i++) 
				pre[i] = tmp_pre[i];
			pre[0] = s;			// 形成回路, s->0
		}
		return ;
	}
	used[s] = 1;
	for (i=0; i<n; i++)		// 搜索 s 邻接点
	{
		// 剪枝, 如果 v 超过了已有的费用, 就没必要继续搜索了
		if (!used[i] && map[s][i] != INF && v < value)
		{
			tmp_pre[i] = s;
			DFS(i, v + map[s][i], f+1);
			tmp_pre[i] = -1;		// 回溯擦除	
		}
	}
	used[s] = 0;		// 擦除标记
}

void show(int *pre)		// 根据前驱输出路径
{
	int i = 0;
	printf("路径: ");
	do {
		printf("%d -> ", i);
		i = pre[i];
	} while (i != 0);
	printf("0\n");
}

int main(void)
{
	int i, j;
	int a, b, c;

	printf("输入顶点数, 边集: ");
	scanf("%d", &n);
	value = INF;
	for (i=0; i<n; i++)		// 初始化, -1 不通
	{
		for (j=0; j<n; j++) map[i][j] = INF;
		used[i] = 0;   tmp_pre[i] = -1;
	}
	while (1)
	{
		scanf("%d %d %d", &a, &b, &c);
		if (!(a || b || c)) break;
		map[a][b] = c;
		map[b][a] = c;		// 无向图
	}
	DFS(0, 0, 0);		// 起点设为 0
	printf("\n最小费用: %d\n", value);
	show(pre);			// 输出路径
	return 0;
}
/*  测试数据
4
0 1 30
0 2 6
0 3 4
1 2 5
2 3 20
3 1 10
0 0 0
*/