
// BF 算法从左向右逐个比较, Horspool 从右向左比较
// 例如
// 匹配串：abcbcsdxzcxx
// 模式串：cbcac
// 从右边的 c 开始比对, 发现 a 与 b 不匹配, 于是在模式串中找 b
// 模式串中如果有 b, 则按 b 对齐, 如下图
// 匹配串：abcbcsdxzcxx
// 模式串：  cbcac
// 于是再从最右边的 c 开始匹配, c 与 d 不匹配
// 在模式串中找 d, 但是模式串中没有 d, 把模式串整个向前滑动
// 匹配串：abcbcsdxzcxx
// 模式串：       cbcac

#include <stdio.h>
#include <string.h>

// 在 r 中找 d, 每次从 d 的右边开始匹配
// 匹配失败向右滑动一段能匹配的距离
int find(char *r, char *d)
{
	int lenr = strlen(r);
	int lend = strlen(d) - 1;
	int i, j, cur;
	char c;
	
	i = j = lend;		// i, j 设在最右边
	while (1)
	{
		cur = j;		// j 当前的位置
		while (r[j] == d[i]) { i--;   j--; }
		if (i == 0) return j + 1;		// 找到
		c = r[j];
		j = 0;
		// d 向右滑动
		while (i > 0 && d[i] != c) { i--;  j++; }
		if (i == 0) j = cur + lend;
		else j = cur + j;
		if (j > lenr) return -1;
		i = lend;
	}
}

int main(void)
{
	char r[300];
	char d[300];
	int pos;

	r[0] = '^';   d[0] = '$';
	// 0 号元素做哨兵, r[0] != d[0]
	while (1)
	{
		printf("输入原串: ");
		scanf("%s", r + 1);
		printf("输入模式串: ");
		scanf("%s", d + 1);
		pos = find(r, d);
		if (pos != -1)
			printf("找到, 起始位置: %d\n\n", pos);
		else 
			printf("未找到!\n\n");
	}
	return 0;
}
