
#include <stdio.h>
#include <math.h>

typedef struct
{
	double x, y;
}point;

const double EPS = 1e-7;

inline double fabs(double x)
{
	return x > 0 ? x : -x;
}

// 计算 a1->a2 与 b1->b2 的夹角的 cos 值
// 利用公式 a . b = |a| * |b| * cos<a, b>
double cos_p(point &a1, point &a2, point &b1, point &b2)
{
	double x1 = a2.x - a1.x;
	double y1 = a2.y - a1.y;
	double x2 = b2.x - b1.x;
	double y2 = b2.y - b1.y;
	double y = x1 * x2 + y1 * y2;
	double da = sqrt((x1 * x1) + (y1 * y1));
	double db = sqrt((x2 * x2) + (y2 * y2));

	return y / (da * db);
}

// 找出 n 个点中 y 值最小的点, y 值一样就找 x 值最小
point lowp(point *v, int n)
{
	int i;
	point r;

	r.y = r.x = 0xFFFFFF;
	for (i=0; i<n; i++)
	{
		if (fabs(r.y - v[i].y) < EPS)
		{
			if (v[i].x < r.x) r = v[i];
		}
		else if (v[i].y < r.y) r = v[i];
	}
	return r;
}

// 找出 n 个点中 y 值最大的点, y 值一样就找 x 值最小
point highp(point *v, int n)
{
	int i;
	point r;
	
	r.y = -0xFFFFFF;
	r.x = 0xFFFFFF;
	for (i=0; i<n; i++)
	{
		if (fabs(r.y - v[i].y) < EPS)
		{
			if (v[i].x < r.x) r = v[i];
		}
		else if (v[i].y > r.y) r = v[i];
	}
	return r;
}

// 判断点 a 点 b 是不是相等
inline int equal(point &a, point &b)
{
	if (fabs(a.x - b.x) > EPS) return 0;
	if (fabs(a.y - b.y) > EPS) return 0;
	return 1;
}

// 求凸包的 Jarvis 进步法, 参考 <算法导论>
void jarvis(point *v, int n, point *stack, int &top)
{
	point high, low, cur;
	int i;
	double jiao, max;
	point tmp, tmp2;

	// 找出最低的点, 和最高的点, 该两点在凸包上
	low = lowp(v, n);
	high = highp(v, n);

	// 从低点往上搜到高点
	top = 0;
	stack[top++] = low;
	cur = low;
	while (!equal(cur, high))
	{
		// 每次找个夹角最小的, 即 cos 值最大的
		tmp = cur;
		tmp2.x = 100000;
		tmp2.y = cur.y;
		max = -0xFFFFF;
		for (i=0; i<n; i++)
		{
			if (!(v[i].y < cur.y))
			{
				jiao = cos_p(cur, tmp2, cur, v[i]);
				// 夹角一样选最远的点
				if (fabs(max - jiao) < EPS)
				{
					if (v[i].y > tmp.y) tmp = v[i];
				}
				else if (jiao > max)
				{
					max = jiao;
					tmp = v[i];
				}
			}
		}
		stack[top++] = tmp;
		cur = tmp;
	}
	// 从高点往低点搜
	while (!equal(cur, low))
	{
		// 每次找个夹角最小的, 即 cos 值最大的
		tmp = cur;
		tmp2.x = -100000;
		tmp2.y = cur.y;
		max = -0xFFFFF;
		for (i=0; i<n; i++)
		{
			if (!(v[i].y > cur.y))
			{
				jiao = cos_p(cur, tmp2, cur, v[i]);
				if (fabs(max - jiao) < EPS)
				{
					if (v[i].y < tmp.y) tmp = v[i];
				}
				else if (jiao > max)
				{
					max = jiao;
					tmp = v[i];
				}
			}
		}
		stack[top++] = tmp;
		cur = tmp;
	}
	top--;
}

int main(void)
{	
	point v[110];
	int n;
	point stack[110];		// 记录凸包
	int top;
	int i;

	while (1)
	{
		printf("输入 n 以及 n 个点: ");
		scanf("%d", &n);
		for (i=0; i<n; i++)
			scanf("%lf %lf", &(v[i].x), &(v[i].y));
		jarvis(v, n, stack, top);
		printf("凸包点集: ");
		for (i=0; i<top; i++)
			printf("(%g, %g), ", stack[i].x, stack[i].y);
		printf("\n\n");
	}
	return 0;
}
/*
11
12 7
24 9
30 5
41 9
80 7
50 87
22 9
45 1
50 7
90 60
102 -22
*/