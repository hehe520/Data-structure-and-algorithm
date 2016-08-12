
// 汉诺塔

#include <stdio.h>

static int count = 0;

inline void move(char a, char b)
{
	printf("%c -> %c\n", a, b);
	count++;
}

void hanio(char a, char b, char c, int n)
{
	if (n == 1) move(a, c);
	else 
	{
		hanio(a, c, b, n-1);	// 第 n-1 块, a -> b
		move(a, c);				// 第 n 块, a -> c
		hanio(b, a, c, n-1);	// 第 n-1 块, b -> c
	}
}

int main(void)
{
	int n;

	printf("输入 n : ");
	scanf("%d", &n);
	// 将 a 上的塔移到 c 上
	count = 0;
	hanio('a', 'b', 'c', n);
	printf("次数: %d\n", count);

	return 0;
}