
// 求点到线段的最短距离

#include <stdio.h>
#include <math.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) > (b) ? (b) : (a))

typedef struct
{
	double x, y;
}point;

const double EPS = 1e-7;

inline double fabs(double x)
{
	return x > 0 ? x : -x;
}

// 判断 q 在不在 p1, p2 的线段上
int on_segment(point &p1, point &p2, point &q)
{
	double x1 = MIN(p1.x, p2.x);
	double x2 = MAX(p1.x, p2.x);
	double y1 = MIN(p1.y, p2.y);
	double y2 = MAX(p1.x, p2.x);

	// 首先点要在线段的范围内 [x1..x2], [y1..y2]
	if (x1 <= q.x && q.x <= x2 
		&& y1 <= q.y && q.y <= y2)
	{
		// 在判断 q->p1,  p2->p1 的叉积是否为 0
		// 叉积为 x1 * y2 - x2 * y1
		x1 = p1.x - q.x;
		y1 = p1.y - q.y;
		x2 = p1.x - p2.x;
		y2 = p1.y - p2.y;
		if (fabs(x1 * y2 - x2 * y1) < EPS) return 1;
	}
	return 0;		// 不在
}

// 计算 a->b, a->k 的点积
double dianji(point &a, point &b, point &k)
{
	double x1 = b.x - a.x;
	double y1 = b.y - a.y;
	double x2 = k.x - a.x;
	double y2 = k.y - a.y;

	return x1 * x2 + y1 * y2;
}

// 判断 k 垂直与 a-b 的投影在不在 a-b 上
int on_line(point &a, point &b, point &k)
{
	if (dianji(a, k, b) < 0) return 0;
	if (dianji(b, a, k) < 0) return 0;
	return 1;
}

// 计算三角形面积
double area(point &a, point &b, point &k)
{
	double x1 = b.x - a.x;
	double y1 = b.y - a.y;
	double x2 = k.x - a.x;
	double y2 = k.y - a.y;
	double s;
	
	s = x1 * y2 - x2 * y1;
	s = fabs(s);
	return s / 2.0;
}

// 求两点间距离
double dis(point &a, point &b)
{
	return sqrt((a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y));
}

// 求点 k 到线段 a-b 的最短距离
double distance(point &a, point &b, point &k)
{
	double d1, d2;

	// 点在线段上, 包括端点
	if (on_segment(a, b, k)) return 0;

	// 点垂直与线段的投影在线段上
	if (on_line(a, b, k))
	{
		// 将点的线段的两点连成一个三角形
		// 三角形的高即是最短距离, 2 * 面积 / 底边
		return 2 * area(a, b, k) / dis(a, b);
	}
	else 
	{	// 点的垂直距离不在线段上, 枚举到两个端点的距离
		d1 = dis(a, k);
		d2 = dis(b, k);
		return d1 > d2 ? d2 : d1;
	}
}

int main(void)
{
	point a, b;
	point k;
	double dis;

	while (1)
	{
		printf("输入一条线段和一个点: ");
		scanf("%lf %lf %lf %lf %lf %lf", 
			&a.x, &a.y, &b.x, &b.y, &k.x, &k.y);
		dis = distance(a, b, k);
		printf("最短距离: %g\n\n", dis);
	}
	return 0;
}