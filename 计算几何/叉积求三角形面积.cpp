
// 叉积的几何意义: a x b 表示 a 和 b 所张成的平行四边形的有向面积
// 那么除以 2 就是平行四边形的一半, 三角形的面积

#include <stdio.h>

const double EPS = 1e-8;

double fabs(double x)
{
	return x > 0 ? x : -x;
}

// 判断三点是否共线, 共线返回 1
int oneline(double x0, double y0, 
			double x1, double y1, 
			double x2, double y2)
{
	// 拿 (x0, y0)->(x2, y2), (x1, y1)->(x2, y2) 做叉积
	// 叉积为 0 则三点共线
	x1 = x1 - x0;   y1 = y1 - y0;
	x2 = x2 - x0;   y2 = y2 - y0;
	if (fabs(x1 * y2 - x2 * y1) < EPS) return 1;
	else return 0;
}

// 由三个点的坐标计算面积
double area(double *x, double *y)
{
	// 拿其中两条边做向量, 进行叉积
	// 拿 (x0, y0)->(x2, y2), (x1, y1)->(x2, y2)
	double x1, x2, y1, y2;

	x1 = x[2] - x[0];   y1 = y[2] - y[0];
	x2 = x[1] - x[0];   y2 = y[1] - y[0];
	return fabs(x1 * y2 - x2 * y1) / 2.0;
}

int main(void)
{
	double x[3], y[3];	// (x[i], y[i]) 是一对坐标

	while (1)
	{
		printf("输入三角形的三个点: ");
		scanf("%lf %lf %lf %lf %lf %lf", 
			x, y, x+1, y+1, x+2, y+2);
		// 先判断是不是三点共线了
		if (oneline(x[0], y[0], x[1], y[1], x[2], y[2]))
			printf("错误, 三点共线\n\n");
		else
			printf("面积为: %g\n\n", area(x, y));
	}
	return 0;
}