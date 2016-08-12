
// 判断射线与多边形相交
// 输入多个多边形, 输入一条射线, 判断相交的多边形
// 判断射线与线段相交:
// 叉积判断线段横跨射线两端
// 点积判断线段两点在射线的起点"前面"

#include <stdio.h>
#include <malloc.h>

typedef struct		// 点
{
	double x, y;
}point;

typedef struct		// 多边形
{
	point *base;
	int n;
}duo;

// 判断射线 p1->p2,  与线段 q1-q2 是否相交, 相交返回 1
// 首先线段 q1-q2 的两点要横跨射线 p1->p2
// 并且两点不能在射线起点的"后面", 即点积不能小于 0
int shexian(point &p1, point &p2, point &q1, point &q2)
{
	double x1, y1, x2, y2, x3, y3;

	// (x1, y1) 是 p1->q1, (x2, y2) 是 p1->q2, (x3, y3) 是 p1->p2
	// 叉积的乘积要小于等于 0, 等于 0 不能直接判断
	x1 = q1.x - p1.x;   y1 = q1.y - p1.y;
	x2 = q2.x - p1.x;   y2 = q2.y - p1.y;
	x3 = p2.x - p1.x;   y3 = p2.y - p1.y;
	if ((x1*y3 - x3*y1) * (x2*y3 - x3*y2) > 0) return 0;

	// 然后用点集判断 p1->q1 与 p1->p2 的夹角是不是小于等于 90
	if (x1 * x3 + y1 * y3 < 0) return 0;
	if (x2 * x3 + y2 * y3 < 0) return 0;
	return 1;
}

// 输出 n 个多边形中与射线相交的多边形的下标
// 没有则不输出
// 射线 a -> b, a - b 不是线段, 不能用两线段相交
// 如果多边形中有一条线段与该射线相交, 则输出
void find(duo *v, int n, point &a, point &b, int *result)
{
	int i, j, k;
	int top = 0;

	for (i=0; i<n; i++)		// 遍历 n 个多边形
	{
		for (j=0; j<v[i].n; j++)	// 遍历 v[i] 的所有边
		{
			k = (j + 1) % v[i].n;
			// 包括端点的相交
			if (shexian(a, b, v[i].base[j], v[i].base[k]))
			{
				result[top++] = i;
				break;
			}
		}
	}
	result[top] = -1;		// 标志结束
}

int main(void)
{
	point a, b;		// 射线 a -> b 沿着 b 无限延伸
	duo *v;
	int n, i, j;
	int result[300];	// 记录结果

	while (1)
	{
		printf("输入多边形个数: ");
		scanf("%d", &n);
		v = (duo *)malloc(n * sizeof(duo));
		for (i=0; i<n; i++)
		{
			printf("逆时针输入第 %d 个多边形: ", i + 1);
			scanf("%d", &(v[i].n));
			v[i].base = (point *)malloc(v[i].n * sizeof(point));
			for (j=0; j<v[i].n; j++)
				scanf("%lf %lf", &(v[i].base[j].x), &(v[i].base[j].y));
		}
		printf("输入射线的起点: ");
		scanf("%lf %lf", &a.x, &a.y);
		printf("输入射线的方向(某点): ");
		scanf("%lf %lf", &b.x, &b.y);
		
		// 输出相交多边形的下标, 没有则不输出
		find(v, n, a, b, result);
		if (result[0] == -1) printf("没有相交的多边形\n");
		else 
		{
			printf("\n相交的多边形: ");
			i = 0;
			while (result[i] != -1) 
				printf("%d, ", result[i++]);
			printf("\n\n");
		}
		// 释放资源
		for (i=0; i<n; i++) 
			free(v[i].base);
		free(v);
	}
	return 0;
}
/*		一组测试数据
4
4
1 1 2 1 2 2 1 2
4
3 0 4 0 4 1 3 1
4
6 0 7 1 6 2 5 1
4
8 -1 9 -1 9 1 8 1
0 0
1 0
*/