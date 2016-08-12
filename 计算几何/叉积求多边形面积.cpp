
// 叉积求多边形面积
// 叉积的几何意义, a x b 表示 a, b 张成的平行四边形的面积
// 除以 2 即是三角形的面积

#include <stdio.h>

#define NUM 400

typedef struct
{
	double x, y;
}point;

// 求多边形面积, 返回面积
// 将多边形化成多个三角形来求
double area(point *p, int n)
{
	double sum = 0.0;
	double x1, y1, x2, y2;
	point d = p[0];
	int i;

	// n 边形有 n - 2 个三角形, i 从 2 开始循环
	for (i=2; i<n; i++)
	{
		x1 = p[i-1].x - d.x;
		y1 = p[i-1].y - d.y;
		x2 = p[i].x - d.x;
		y2 = p[i].y - d.y;
		sum += x1 * y2 - x2 * y1;
	}
	sum = sum > 0 ? sum : -sum;
	return sum / 2.0;
}

int main(void)
{
	int n, i;
	point p[NUM];

	while (1)
	{
		// 按顺时针输入坐标, 注意不要出现 n 点共线
		printf("输入 n, 以及顺时针输入 n 个点坐标: ");
		scanf("%d", &n);
		if (n < 3) return 0;	// 小于 3 个点
		for (i=0; i<n; i++)
			scanf("%lf %lf", &(p[i].x), &(p[i].y));
		printf("面积为: %g\n\n", area(p, n));
	}
	return 0;
}
