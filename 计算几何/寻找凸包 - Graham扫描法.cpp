
#include <stdio.h>
#include <math.h>

typedef struct
{
	double x, y;
}point;

const double EPS = 1e-7;

inline double fabs(double x)	// 求绝对值
{
	return x > 0 ? x : -x;
}

inline void swap(point &a, point &b)	// 两点交换
{
	point tmp;
	tmp = a;
	a = b;
	b = tmp;
}

// 计算 (a->b) X (c->d) 的叉积
// 若返回值小于 0, (a->b) 在 (c->d) 左边
double xmul(point &a1, point &a2, point &b1, point &b2)
{
	return (a2.x-a1.x) * (b2.y-b1.y) - (b2.x-b1.x) * (a2.y-a1.y);
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

// 按极角排序, 冒泡排序, 或用快排
void sort(point *v, int n)
{
	point low = lowp(v, n);
	point t;
	double jiao1, jiao2;
	int flag = 0;
	int i, j;

	// 找到最低点, 从最低点发出一条右水平射线 low->t
	t.x = 100000;
	t.y = low.y;
	low.x -= 1;
	for (i=0; i<n; i++)
	{
		flag = 0;
		for (j=1; j<n; j++)
		{
			jiao1 = cos_p(low, t, low, v[j-1]);
			jiao2 = cos_p(low, t, low, v[j]);
			if (fabs(jiao1 - jiao2) < EPS)
			{
				if (v[j-1].y > v[j].y)
					swap(v[j-1], v[j]);
				flag = 1;
			}
			else if (jiao1 < jiao2)
			{
				swap(v[j-1], v[j]);
				flag = 1;
			}
		}
		if (!flag) break;
	}
}

// graham 扫描法求凸包
void graham(point *v, int n, point *stack, int &top)
{
	int i;
	int cur;

	// 所有点的极角从小到大排序
	sort(v, n);
	for (top=i=0; i<3; i++)
		stack[top++] = v[i];
	cur = 3;
	// 从第四个点开始, 搜到最低点就结束
	while (cur < n)
	{
		// 叉积小于 0, 逆时针, 即左拐, 等于 0 的情况, 共线
		while (xmul(stack[top-2], stack[top-1], stack[top-2], v[cur]) < 0)
			top--;
		stack[top++] = v[cur++];
	}
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
		graham(v, n, stack, top);
		printf("\n凸包点集: ");
		for (i=0; i<top; i++)
			printf("(%g, %g), ", stack[i].x, stack[i].y);
		printf("\n\n");
	}
	return 0;
}
/*
10
12 7
24 9
41 9
80 7
50 87
22 9
45 1
90 60
72 36
59 28
*/