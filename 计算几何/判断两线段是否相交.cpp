
// 判断线段 P(p1, p2), Q(q1, q2) 是否相交
// 线段 P 有两个端点 p1, p2 判断 p1, p2 在不在 Q 的两端
// 在 Q 中取一点 q1 组成两个向量 q1->p1, q1->p2
// 这两个向量的与 q1->q2 做叉积, 如果 p1, p2 在两端的话
// 那么 q1->p1, q1->p2 肯定是一个在 q1->q2 的顺时针, 一个在逆时针
// 那么将 q1->p1, q1->p2 分别与 q1->q2 叉积再相乘肯定小于等于 0
// 等于 0 的情况是线段重合
// 在把 Q 的两个端点进行上述判断, 如果通过则相交
// 判断直线与线段相交的话, 只需判断线段的两个点是不是在直线的两端

#include <stdio.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) > (b) ? (b) : (a))

typedef struct
{
	double x, y;
}point;

const double EPS = 1e-8;

inline double fabs(double x)
{
	return x > 0 ? x : -x;
}

// 判断点 q 在不在 p1-p2 的线段上
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

// 判断 p1, p1 在不在 Q 的两端, 在返回 1
int two_side(point &p1, point &p2, point &q1, point &q2)
{
	// 如果向量 q1->p1 在 q1->q2 的顺时针(或逆时针), 则叉积为正
	// 而 q1->p2 在 q1->q2 的逆时针(或顺时针), 则叉积为负
	// 所以叉积的乘积小于 0, 等于 0 是重合
	double x1, y1, x2, y2, x3, y3;

	// q1->p1 (x1, y1), q1->p2 (x2, y2), q1->q2 (x3, y3)
	x1 = p1.x - q1.x;   y1 = p1.y - q1.y;
	x2 = p2.x - q1.x;   y2 = p2.y - q1.y;
	x3 = q2.x - q1.x;   y3 = q2.y - q1.y;
	if ((x1*y3 - x3*y1) * (x2*y3 - x3*y2) < 0) return 1;
	return 0;
}

// 判断线段 (p1, p2) (q1, q2) 是否相交, 相交返回 1
int segment_intersect(point &p1, point &p2, point &q1, point &q2)
{
	// 先判断 p1, p2 在不在 Q 的两端
	// 再判断 q1, q2 在不在 P 的两端
	// 判断线段与直线相交的话, 只需判断一次
	if (two_side(p1, p2, q1, q2)
		&& two_side(q1, q2, p1, p2)) 
		return 1;

	// 当一个点在另一条线段上的时候, 叉积的乘积为 0
	// 由于精度问题, 还需分别判断四个点在不在线段上
	if (on_segment(p1, q1, q2)) return 1;
	else if (on_segment(p2, q1, q2)) return 1;
	else if (on_segment(q1, p1, p2)) return 1;
	else if (on_segment(q2, p1, p2)) return 1;

	return 0;
}

int main(void)
{
	point p1, p2;
	point q1, q2;

	while (1)
	{
		// 输入顺序 p1.x, p1.y, p2.x, p2.y
		printf("输入线段 p 的两个端点: ");
		scanf("%lf %lf %lf %lf", &p1.x, &p1.y, &p2.x, &p2.y);
		printf("输入线段 q 的两个端点: ");
		scanf("%lf %lf %lf %lf", &q1.x, &q1.y, &q2.x, &q2.y);
		if (segment_intersect(p1, p2, q1, q2))
			printf("相交\n\n");
		else 
			printf("不相交\n\n");
	}
	return 0;
}