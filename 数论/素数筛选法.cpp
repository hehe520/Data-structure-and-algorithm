
// 素数筛选
// 如果 k 是素数, 那么以 k 为因子的数不是素数

#include <stdio.h>

#define NUM 20000		// 两万以内的素数
int map[NUM];			// 筛子

void initialize()
{
	int i, j;

	for (i=0; i<NUM; i++) map[i] = 1;
	map[0] = map[1] = 1;

	for (i=2; i<NUM; i++)
	{
		if (map[i])		// i 是素数
		{
			for (j=i+i; j<NUM; j+=i) map[j] = 0;
		}
	}
}

int main(void)
{
	int i;

	initialize();		// 初始化
	printf("素数:\n");
	for (i=2; i<200; i++)
	{
		if (map[i]) printf("%d, ", i);
	}
	return 0;
}