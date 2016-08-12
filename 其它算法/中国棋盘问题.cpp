
// 中国棋盘问题 - 编程之美问题 1.2
// 只能用 1 字节的储存空间

#include <stdio.h>

int main(void)
{
	struct
	{
		unsigned char a : 4;
		unsigned char b : 4;
	}i;
	for (i.a=1; i.a<=9; i.a++)
		for (i.b=1; i.b<=9; i.b++)
			if (i.a % 3 != i.b % 3)
				printf("%d, %d\n", i.a, i.b);
	return 0;
}
