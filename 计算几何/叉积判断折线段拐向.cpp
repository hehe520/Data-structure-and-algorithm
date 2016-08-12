
// 设 P, Q 是两个线段
// 若 P x Q > 0 , 则 P 在 Q 的顺时针方向
// 若 P x Q < 0 , 则 P 在 Q 的逆时针方向
// 若 P x Q = 0 , 则 P 与 Q 共线, 也可能同向也可能反向

#include <stdio.h>

const double EPS = 1e-8;

int abs(double x)
{
	return x > 0 ? x : -x;
}

// 计算 P, Q 叉积, P x Q = px * qy - py * qx
double cmul(double px, double py, double qx, double qy)
{
	return px * qy - py * qx;
}

int main(void)
{
	double px1, py1, px2, py2;
	double qx1, qy1, qx2, qy2;
	double ji;

	while (1)
	{
		// 输入顺序 p1(x1, y1)  p2(x2, y2), 向量 P = p1->p2 
		printf("输入线段 P 的两个端点: ");
		scanf("%lf %lf %lf %lf", &px1, &py1, &px2, &py2);
		printf("输入线段 Q 的两个端点: ");
		scanf("%lf %lf %lf %lf", &qx1, &qy1, &qx2, &qy2);
		// 用 p2 - p1
		px1 = px2 - px1;   py1 = py2 - py1;
		qx1 = qx2 - qx1;   qy1 = qy2 - qy1;
		ji = cmul(px1, py1, qx1, qy1);
		if (abs(ji) < EPS) printf("P 与 Q 共线\n\n");
		else if (ji > 0) printf("P 在 Q 的顺时针方向\n\n");
		else if (ji < 0) printf("P 在 Q 的逆时针方向\n\n");
	}
	return 0;
}