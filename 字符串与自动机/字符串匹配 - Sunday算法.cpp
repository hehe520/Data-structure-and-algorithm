
// Sunday 的算法和 Horspool 类似
// 比如：
// 匹配串：abcbczdxzc
// 模式串：zbcac
// 从右往左比较, c 与 c 匹配, a 与 b 就不匹配了
// 于是找到匹配串中 c 的下一个字符 z
// 看看模式串中有没有 z, 如果有, 按照 z 对齐
// 匹配串：abcbczdxzc
// 模式串：     zbcac
// 如果模式串中的没有 z, 例如
// 匹配串：abcbcedxzcs
// 模式串：zbcac
// e 不在模式串中, 那么就跳过 e
// 匹配串：abcbcedxzcs
// 模式串：      zbcac

#include <stdio.h>
#include <string.h>

// 在 r 中找 d, 返回匹配位置
int find(char *r, char *d)
{
	int lenr = strlen(r);
	int lend = strlen(d) - 1;
	int i, j, cur;
	char c;

	i = j = lend;
	while (1)
	{
		cur = j;
		while (r[j] == d[i]) { i--;  j--; }
		if (i == 0) return j + 1;
		i = lend;				// i 回到最右边
		c = r[cur + 1];			// 寻找 cur + 1 的字符
		while (i > 0 && d[i] != c) i--;
		if (i == 0) j = cur + lend + 1;
		else j = cur + lend - i + 1;
		if (j > lenr) return -1;
		i = lend;
	}
}

int main(void)
{
	char r[300];
	char d[300];
	int pos;

	r[0] = '#';   d[0] = '^';
	// 0 号单元做哨兵, r[0] != d[0]
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
