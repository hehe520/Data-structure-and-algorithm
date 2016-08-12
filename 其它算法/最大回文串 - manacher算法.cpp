
// 输入 XMADAMYX,  输出 MADAM
// 输入 AYXMAAMXY  输出 YXMAAMXY
// 算法:
// XMADAMYX 处理成: #X#M#A#D#A#M#Y#X# 
// 接着枚举回文串中心, 用 p[] 记录回文长度
// 找最大的 p[]

#include <stdio.h>
#include <string.h>

void out(char *str)
{
	int i, j, k;
	char buf[1000];
	int p[1000];
	int len = strlen(str);
	int max = -1, pos;

	for (j=i=0; i<len; i++, j++)	// 加 #
	{
		buf[j++] = '#';
		buf[j] = str[i];
	}
	buf[j++] = '#';
	buf[j] = '\0';
	for (i=0; i<=j; i++)		// 枚举中心
	{
		k = 0;
		while (1)
		{
			if (i+k<=j && i-k>=0 && buf[i+k] == buf[i-k])
				k++;
			else break;
		}
		p[i] = k;
	}
	for (i=0; i<=j; i++)		// 查找最大回文
	{
		if (p[i] > max)
		{ max = p[i];   pos = i; }
	}
	max--;				// 回文区间 [pos-max, pos]
	pos += max;
	max *= 2;
	for (i=pos-max; i<pos; i++)
	{
		if (buf[i] != '#') putchar(buf[i]);
	}
	putchar('\n');
}

int main(void)
{
	char buf[1000];

	while (1)
	{
		printf("输入一个串: ");
		scanf("%s", buf);
		out(buf);
	}
	return 0;
}