// 重写 prim 最小生成树

#include <stdio.h>
#include <string.h>

#define NUM 30		// 最多 30 个顶点
#define INF 0xFFFFF

// prim 算法适合稠密图, 其搜索路径是深搜, 
// prim 算法类似 dijskra 算法, 不断去松弛一个数组 min_w
// min_w[k] 表示 到 k 点的最小边的权值是 min_w[k]
// 同时对于访问过的点 k, 要进行访问标记
int prim(int (*m)[NUM], char *v, int len)
{
	int sum = 0;
	int min_w[NUM];		// min_w 记录最小权值
	char used[NUM];		// 访问标记
	int pre[NUM];		// 记录前驱结点, 获得路径
	int min, pos, i, j;

	for (i=0; i<len; i++) min_w[i] = INF;
	for (i=0; i<len; i++) used[i] = 0;

	min_w[0] = 0;	// 设置 0 为搜索起点
	// n 个点, 至少需要 n-1 条边, 才能连成一个图
	// 由于第一个循环并没有找到边, 所以循环 n 次
	for (j=0; j<len; j++)
	{
		// 找一个最小权值的边, 记录下权值
		min = INF;
		for (i=0; i<len; i++)
		{
			if (!used[i] && min_w[i] < min) 
			{
				min = min_w[i];
				pos = i;		// 记录该点 i
			}
		}
		if (min == INF) return -1;		// 没有找到, 不是连通图
		used[pos] = 1;			// 选中 pos 点, 同时要标记
		sum += min;				// 累加权值
		if (pos != 0) printf("(%c -> %c), ", v[ pre[pos] ], v[pos]);

		// 用这个点 (pos) 去松弛 min 中的其它边
		for (i=0; i<len; i++)
		{
			if (m[pos][i] < min_w[i]) 
			{
				min_w[i] = m[pos][i];
				pre[i] = pos;
			}
		}
	}
	return sum;
}

// 因为结点名称采用了字符 'a''b''c', 所以要找对应的位置
// 如果是用数字, 则数字直接作为矩阵的坐标位置
inline int find_addr(char *vex, int len, char a)
{
	int i;
	for (i=0; i<len; i++)
		if (vex[i] == a) return i;
	return -1;
}

int main(void)
{
	char vex[NUM];
	int matrix[NUM][NUM];
	int len, wight;
	char a, b;
	int add1, add2;
	int sum = 0;

	printf("建立一个无向图, 请输入顶点集: ");
	scanf("%s", vex);   fflush(stdin);
	len = strlen(vex);
	for (a=0; a<len; a++)
		for (b=0; b<len; b++) matrix[a][b] = INF;
	printf("请输入边集: ");
	while (1)
	{
		fflush(stdin);
		scanf("%c %c %d", &a, &b, &wight);
		if (a == '0' && b == '0' && wight == 0) break;
		add1 = find_addr(vex, len, a);
		add2 = find_addr(vex, len, b);
		if (!(add1 == -1 || add2 == -1) && wight > 0)
		{
			matrix[add1][add2] = wight;
			matrix[add2][add1] = wight;		// 如果是有向图则不要这句
		}
	}
	sum = prim(matrix, vex, len);
	printf("\n最小权值 %d\n", sum);

	return 0;
}
