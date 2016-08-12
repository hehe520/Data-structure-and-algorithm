#include <stdio.h>

inline void move(char a, char b)
{
	printf("%c -> %c\n", a, b);
}

// 返回汉诺塔移动的次数
int hanoi(int n, char a, char b, char c)
{
	static int count = 0;

	if (n > 0)
	{
		hanoi(n-1, a, c, b);	// 要先把 n-1 的移到 b
		count++;
		move(a, c);				// 再把当前 n 移到 c
		hanoi(n-1, b, a, c);	// 再把 n-1 到 c
	}
	return count;
}

int main(void)
{
	int n;

	printf("hanoi 塔问题\n从 a  移到 c, 请输入 n: ");
	scanf("%d", &n);
	n = hanoi(n, 'a', 'b', 'c');
	printf("\n一共移动了 %d 次\n", n);

	return 0;
}
