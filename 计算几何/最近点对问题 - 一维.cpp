
// 分治法求一维最近点对问题
// 在 x 轴上有 n 个点, 求出距离最近两点
// 分成最近点对可能在左区间, 右区间, 或者左右交界处

#include <stdio.h>
#include <stdlib.h>

#define MIN(a, b) ((a.dis) > (b.dis) ? (b) : (a))
#define fabs(a)   ((a) > (0) ? (a) : (-(a)))

// 记录类型 (a, b) 最近点对, 距离为 dis
typedef struct
{
	double a, b;
	double dis;
}recond;

// 在 num 中找最近的两点
recond min_close(double *num, int n)
{
	recond re;
	recond left, right;
	int mid;

	if (n == 1)		// 只有一个点距离无穷大
	{
		re.a = re.b = num[0];
		re.dis = 0xFFFFFF;
		return re;
	}
	else if (n == 2)
	{
		re.a = num[0];
		re.b = num[1];
		re.dis = num[0] - num[1];
		re.dis = fabs(re.dis);
		return re;
	}
	else	// 三个以上的点
	{
		mid = n / 2;		// 划分一半
		left = min_close(num, mid);
		right = min_close(num + mid, n - mid);

		// 找出左区间, 右区间, 跨区间的最小值
		re.a = num[mid];
		re.b = num[mid + 1];
		re.dis = num[mid] - num[mid + 1];
		re.dis = fabs(re.dis);
		re = MIN(re, left);
		re = MIN(re, right);
		return re;
	}
}

// 排序比较函数
int cmp(const void *x, const void *y)
{
	double a, b;

	a = *((double *)x);
	b = *((double *)y);

	return a > b;
}

int main(void)
{
	int n, i;
	double num[300];
	recond d;

	while (1)
	{
		printf("输入 n, 以及 n 个点: ");
		scanf("%d", &n);
		for (i=0; i<n; i++)
			scanf("%lf", num + i);
		qsort(num, n, sizeof(num[0]), cmp);		// 从小到大排序
		d = min_close(num, n);
		printf("\n最近点对: %g, %g\n最近距离: %g\n\n", 
			d.a, d.b, d.dis);
	}
	return 0;
}