
// 矩形是凸集, 所以只要判断矩形的四个点都在另一个矩形内即可

#include <stdio.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) > (b) ? (b) : (a))

typedef struct
{
	double x, y;
}point;

// 判断 k 在不在矩形 a 中, 若 k 在边上返回 0
int dian(point &k, point &a1, point &a2)
{
	double x1 = MIN(a1.x, a2.x);
	double x2 = MAX(a1.x, a2.x);
	double y1 = MIN(a1.y, a2.y);
	double y2 = MAX(a1.y, a2.y);

	if (x1 < k.x && k.x < x2 && y1 < k.y && k.y < y2)
		return 1;
	return 0;
}

// 判断 a 矩形中是否包含 b, 不判断点在边上的情况
int in(point &a1, point &a2, point &b1, point &b2)
{
	int sum = 0;
	double tmp;

	// 分别验证矩形 b 的四个点是不是在 a 中
	sum += dian(b1, a1, a2);
	sum += dian(b2, a1, a2);
	tmp = b1.x;			// 交换 x 值, 变成另一对对角点
	b1.x = b2.x;
	b2.x = tmp;
	sum += dian(b1, a1, a2);
	sum += dian(b2, a1, a2);
	// 四点全部通过, sum == 4
	if (sum == 4) return 1;
	else return 0;
}

int main(void)
{
	point a1, a2, b1, b2;

	while (1)
	{
		// 判断 a 矩形中是否包含 b
		printf("输入矩形 a 对角的两个点: ");
		scanf("%lf %lf %lf %lf", &a1.x, &a1.y, &a2.x, &a2.y);
		printf("输入矩形 b 对角的两个点: ");
		scanf("%lf %lf %lf %lf", &b1.x, &b1.y, &b2.x, &b2.y);
		if (in(a1, a2, b1, b2))
			printf("包含\n\n");
		else 
			printf("不包含\n\n");
	}
	return 0;
}