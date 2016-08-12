
// 在 n 个点中找距离最近的两个点

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <algorithm>

#define INF 0xFFFFFF

typedef struct
{
	double x, y;
}point;

// 记录点 a, b 距离是 dis, 找出最近距离同时找出最近点对
typedef struct
{
	point *a;
	point *b;
	double dis;
}recond;

point p[100010];		// 原数组
point *x[100010];		// x 坐标预排序
point *y[100010];		// y 坐标预排序

inline double dist(point *a1, point *a2)
{
	return sqrt((a1->x-a2->x)*(a1->x-a2->x)+(a1->y-a2->y)*(a1->y-a2->y));
}

inline double fabs(double x)
{
	return x > 0 ? x : -x;
}

int cmpx(const void *x, const void *y)
{
	point *a = (point *)x;
	point *b = (point *)y;

	return (a->x < b->x);
}

int cmpy(const void *x, const void *y)
{
	point *a = (point *)x;
	point *b = (point *)y;

	return (a->y < b->y);
}

// 找最近的点对
recond min_close(int s, int n)
{
	recond r;
	int i, j, top;
	double dis;

	if (n - s == 1)		// 一个点
	{
		r.dis = INF;
		return r;
	}
	else if (n - s == 2)	// 两个点
	{
		r.a = x[0];
		r.b = x[1];
		r.dis = dist(x[0], x[1]); 
		return r;
	}
	else if (n - s == 3)	// 三个点
	{
		r.dis = INF;
		for (i=0; i<3; i++)
		{
			j = (i + 1) % 3;
			dis = dist(x[i], x[j]);
			if (dis < r.dis)
			{
				r.dis = dis;
				r.a = x[i];
				r.b = x[j];
			}
		}
	}
	// 三个以上的点
	int mid = (n + s) / 2;
	recond d1 = min_close(s, mid);
	recond d2 = min_close(mid, n);
	r = d1.dis < d2.dis ? d1 : d2;

	top = 0;
	// 在 x[mid] 中心线, 向左右拓展 r.dis 宽度
	for (i=s; i<n; i++)
		if (fabs(x[i]->x - x[mid]->x) < r.dis)
			y[top++] = x[i];
			// y 记录 x 坐标小于 r.dis 的点
			// 也就是记录落在宽度内的点, 不用判断全部的点
	// 按 y 坐标从小到大排序, 枚举最小距离
	std::sort(y, y + top, cmpy);
	for (i=0; i<top; i++)
	{
		for (j=i+1; j<top; j++)
		{
			if (y[j]->y - y[i]->y < r.dis)
			{
				dis = dist(y[j], y[i]);
				if (dis < r.dis)
				{
					r.dis = dis;
					r.a = y[j];
					r.b = y[i];
				}
			}
			else break;
			// y 坐标是有序的, 一次超界, 后面的都超界
		}
	}
	return r;
}

int main(void)
{
	recond r;
	int n;
	int i;

	while (1)
	{
		printf("输入 n 以及 n 个点: ");
		scanf("%d", &n);
		for (i=0; i<n; i++)
		{
			scanf("%lf %lf", &(p[i].x), &(p[i].y));
			x[i] = p + i;
		}
		std::sort(x, x + n, cmpx);
		r = min_close(0, n);
		printf("最近点对: (%g, %g), (%g, %g)\n", 
			r.a->x, r.a->y, r.b->x, r.b->y);
		printf("最近距离: %g\n\n", r.dis);
	}
	return 0;
}