
// 判断线段在不在多边形内

#include <stdio.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) > (b) ? (b) : (a))

typedef struct
{
	double x, y;
}point;

const double EPS = 1e-7;

// 判断线段 a-b 在不在多边形 v 中
// 首先线段的两个端点要在多边形中, 这是必要条件
// 因为多边形可能是凹集, 线段某一段可能在多边形外面
int in(point *v, int n, point &a, point &b)
{
	// 判断 a 是否在多边形内
	int in_duo(point *v, int n, point &a);
	// 判断线段 a-b, c-d 是否相交
	int xiangjiao(point &a, point &b, point &c, point &d);
	// 判断 k 在不在线段 a-b 上
	int on_segment(point &k, point &a, point &b);
	int i, j, k;

	// 判断 a, b 两点在不在多边形中
	if (!in_duo(v, n, a)) return 0;
	if (!in_duo(v, n, b)) return 0;
	for (i=0; i<n; i++)		// 遍历所有边 v[i]-[j]
	{
		j = (i + 1) % n;
		// 不包括端点的相交
		if (xiangjiao(a, b, v[i], v[j])) return 0;

		// 如果 i 点在线段 a-b 上, 判断是不是内切
		// 判断 i 点前后的两条线段是不是横跨 a-b
		if (on_segment(v[i], a, b))
		{
			k = (i - 1) % n;
			if (xiangjiao(v[j], v[k], a, b))
				return 0;
		}
	}
	return 1;
}

int main(void)
{
	point vex[400];		// 最多 400 个点
	int n, i;
	point a, b;		// 线段 a-b

	while (1)
	{
		printf("逆时针输入一个多边形: ");
		scanf("%d", &n);		// 顶点数
		for (i=0; i<n; i++)
			scanf("%lf %lf", &vex[i].x, &vex[i].y);
		printf("输入一个线段: ");
		scanf("%lf %lf %lf %lf", &a.x, &a.y, &b.x, &b.y);
		if (in(vex, n, a, b))
			printf("在\n\n");
		else 
			printf("不在\n\n");
	}
	return 0;
}


inline double fabs(double x)
{
	return x > 0 ? x : -x;
}

// 判断 k 在不在线段 a-b 上
int on_segment(point &k, point &a, point &b)
{
	double x1 = MIN(a.x, b.x);
	double x2 = MAX(a.x, b.x);
	double y1 = MIN(a.y, b.y);
	double y2 = MAX(a.y, b.y);

	if (x1 <= k.x && k.x <= x2 && y1 <= k.y && k.y <= y2)
	{
		// a->k, a->b 两个向量做叉积, 判断是不是 0
		x1 = k.x - a.x;
		y1 = k.y - a.y;
		x2 = b.x - a.x;
		y2 = b.y - a.y;
		if (fabs(x1 * y2 - x2 * y1) < EPS)
			return 1;
	}
	return 0;
}

// 判断 p1, p1 在不在 Q 的两端, 在返回 1
int two_side(point &p1, point &p2, point &q1, point &q2)
{
	double x1, y1, x2, y2, x3, y3;

	// q1->p1 (x1, y1), q1->p2 (x2, y2), q1->q2 (x3, y3)
	x1 = p1.x - q1.x;   y1 = p1.y - q1.y;
	x2 = p2.x - q1.x;   y2 = p2.y - q1.y;
	x3 = q2.x - q1.x;   y3 = q2.y - q1.y;
	if ((x1*y3 - x3*y1) * (x2*y3 - x3*y2) < 0) return 1;
	return 0;
}

// 判断线段 a-b, c-d 是否相交, 不判断两个端点
int xiangjiao(point &a, point &b, point &c, point &d)
{
	if (two_side(a, b, c, d) && two_side(c, d, a, b))
		return 1;
	return 0;
}

// 判断 k 点在不在多边形 s 内
int in_duo(point *s, int n, point &k)
{
	point t;		// 左水平射线 t <- k
	point tmp;
	int count = 0;
	int i, j;

	t.y = k.y;
	t.x = -0xFFFFF;
	for (i=0; i<n; i++)		// 遍历每一条边 s[i-1] - s[i]
	{
		j = (i + 1) % n;
		if (on_segment(k, s[i], s[j]))	// 点 k 在边上
			return 1;
		else 
		{
			// 判断线段是不是水平的, 水平线段忽略
			if (fabs(s[i].y - s[j].y) > EPS) 
			{
				// 找出线段中较高的点 tmp
				if (s[i].y < s[j].y) tmp = s[j];
				else tmp = s[i];
				// 判断 tmp 在不在射线上
				if (on_segment(tmp, k, t)) 
					count++;
				// 或者射线与边相交
				else if (xiangjiao(k, t, s[i], s[j]))
					count++;
			}
		}
	}
	if (count % 2 == 1) return 1;
	else return 0;
}
