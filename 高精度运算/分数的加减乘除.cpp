
#include <stdio.h>

typedef struct
{
	int z;		// 分子
	int m;		// 分母
}fen;

int gcd(int a, int b)		// 最大公约数
{
	int r;

	while (b)
	{
		r = a % b;
		a = b;
		b = r;
	}
	return a;
}

void simple(fen *f)			// 分数化简
{
	int d = gcd(f->z, f->m);

	f->z /= d;
	f->m /= d;
	if (f->z < 0 && f->m < 0)
	{ f->z = -f->z;   f->m = -f->m; }
}

fen add(fen f1, fen f2)		// 加法, 同时化简
{
	fen f3;
	int m1 = f1.m;
	int m2 = f2.m;

	f1.m *= m2;
	f1.z *= m2;
	f2.m *= m1;
	f2.z *= m1;
	f3.z = f1.z + f2.z;
	f3.m = f1.m;
	simple(&f3);

	return f3;
}

fen sub(fen f1, fen f2)		// 减法转换为加法
{
	f2.z = -f2.z;
	return add(f1, f2);
}

fen mul(fen f1, fen f2)		// 乘法
{
	f1.z *= f2.z;
	f1.m *= f2.m;
	simple(&f1);

	return f1;
}

fen div(fen f1, fen f2)		// 除法转换为乘法
{
	int tmp;

	tmp = f2.z;
	f2.z = f2.m;
	f2.m = tmp;

	return mul(f1, f2);
}

int main(void)
{
	fen f1, f2, f3;

	while (1)
	{
		// 分母不能为 0, 分子为 0 分数值为 0
		printf("\n输入分子, 分母: ");
		scanf("%d %d", &(f1.z), &(f1.m));
		printf("输入分子, 分母: ");
		scanf("%d %d", &(f2.z), &(f2.m));
		f3 = add(f1, f2);
		printf("相加: %d / %d\n", f3.z, f3.m);
		f3 = sub(f1, f2);
		printf("相减: %d / %d\n", f3.z, f3.m);
		f3 = mul(f1, f2);
		printf("相乘: %d / %d\n", f3.z, f3.m);
		f3 = div(f1, f2);
		printf("相除: %d / %d\n", f3.z, f3.m);
	}
	return 0;
}