
// 随机数发生器 random()

#include <stdio.h>
#include <time.h>

#define A 48271
#define M 2147483647		// (2^31 - 1)

__int64 seed;

// 初始化
void initialize(int s)
{
	seed = s;
}

int random()
{
	seed = (seed * A) % M;
	return (int)seed;
}

int main(void)
{
	int i;
	
	initialize(time(NULL));		// 时间做种子
	// 产生 30 个 0 - 999 的随机数
	for (i=0; i<30; i++)
		printf("%d\n", random() % 999);
	return 0;
}