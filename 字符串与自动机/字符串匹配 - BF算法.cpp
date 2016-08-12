
// BF 算法, 从左向右一位一位比较, 匹配失败时, 模式串右移一位

#include <stdio.h>
#include <string.h>

// 在 r 中找 d, 返回找到的位置
int find(char *r, char *d)
{
	int lenr = strlen(r);
	int lend = strlen(d);
	int i = 0, j = 0;

	while (i < lenr)
	{
		if (r[i] == d[j]) { i++;  j++; }
		else 
		{
			i = i - j + 1;   //回溯 i 指针
			j = 0;
		}	
		if (j == lend) break;
	}
	if (j != lend) return -1;
	else return i - j + 1;
}

int main(void)
{
	char r[300];
	char d[300];
	int pos;

	while (1)
	{
		printf("输入原串: ");
		scanf("%s", r);
		printf("输入模式串: ");
		scanf("%s", d);
		pos = find(r, d);
		if (pos != -1)
			printf("找到, 起始位置: %d\n\n", pos);
		else 
			printf("未找到!\n\n");
	}
	return 0;
}
