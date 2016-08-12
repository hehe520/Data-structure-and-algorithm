
// 计算圆心到矩形四条边的垂直距离, 找出最小的距离 m
// 判断半径是否小于 m

#include <stdio.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) > (b) ? (b) : (a))
#define fabs(a) ((a) > 0 ? (a) : (-a))
#define EPS (1e-8)

typedef struct
{
	double x, y;
}point;

// 判断圆在不在矩形内
int in(point &k, double &r, point &a1, point &a2)
{
	double x1 = MIN(a1.x, a2.x);
	double x2 = MAX(a1.x, a2.x);
	double y1 = MIN(a1.y, a2.y);
	double y2 = MAX(a1.y, a2.y);

	// 计算圆心到四条边的距离
	x1 = k.x - x1;
	x2 = x2 - k.x;
	y1 = k.y - y1;
	y2 = y2 - k.y;
	// 找出最短的距离与半径比较
	x1 = MIN(x1, x2);
	y1 = MIN(y1, y2);
	x1 = MIN(x1, y1);
	// r == x1 的情况要用 EPS 判断
	if (r < x1) return 1;
	else if (fabs(r - x1) < EPS) return 1;
	else return 0;
}

int main(void)
{
	point a1, a2, k;
	double r;

	while (1)
	{
		printf("输入矩形的两个对角顶点: ");
		scanf("%lf %lf %lf %lf", &a1.x, &a1.y, &a2.x, &a2.y);
		printf("输入圆心和半径: ");
		scanf("%lf %lf %lf", &k.x, &k.y, &r);
		if (in(k, r, a1, a2)) printf("包含\n\n");
		else printf("不包含\n\n");
	}
	return 0;
}
