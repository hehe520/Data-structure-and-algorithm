
// 判断点在不在线段上, 设点为 Q, 线段为 P1, P2
// 判断在不在直线上, 只需在直线上任取两点 p1, p2
// 再判断 q, p1, p2 三点是否共线, 即向量 (q->p1), (p1->p2) 的叉积是不是 0

#include <stdio.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) > (b) ? (b) : (a))

const double EPS = 1e-8;

typedef struct		// 点类型
{
	double x;
	double y;
}point;

inline double fabs(double x)
{
	return x > 0 ? x : -x;
}

// 判断 q 在不在 p1, p2 的线段上
int on_segment(point &q, point &p1, point &p2)
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

int main(void)
{
	point q, p1, p2;

	while (1)
	{
		printf("输入线段 P 的两个端点: ");
		scanf("%lf %lf %lf %lf", &p1.x, &p1.y, &p2.x, &p2.y);
		printf("输入一个点: ");
		scanf("%lf %lf", &q.x, &q.y);
		if (on_segment(q, p1, p2))
			printf("在\n\n");
		else 
			printf("不在\n\n");
	}
	return 0;
}