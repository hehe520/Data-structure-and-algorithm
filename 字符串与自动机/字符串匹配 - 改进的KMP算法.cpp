#include <stdio.h>
#include <string.h>

// 在 a 中的第 pos 之后的位置找 b 串
int kmp(char *a, char *b, int pos)
{
	int next[50] = {0};
	int lena = strlen(a);
	int lenb = strlen(b);
	int i, j;

	// 生成改进的 next 数组
	//  a  a  a  b
	// -1  0  1  2    原来的情况
	// -1 -1 -1  2    改进后的
	next[0] = -1;	// -1 表示不能再回溯 j 了
	j = -1;		// 开始 j==-1 表示字符串到头了，
	i = 0;		// i 从第一个字符开始
	while (i < lena)
	{
		if (j == -1 || b[i] == b[j])
		{
			i++;   j++;
			if (b[i] == b[j]) next[i] = j;
			else next[i] = next[j];
		}
		else j = next[j];	// 匹配失败时, 回到 next[j]
	}

	// 开始 kmp 算法
	i = pos;
	j = 0;
	while (i < lena && j < lenb)
	{
		if (j == -1 || a[i] == b[j]) {i++;  j++;}
		else j = next[j];	// 回溯 j 指针
	}
	if (j == lenb) return i - j + 1;
	return -1;
}

int main(void)
{
	char a[500];
	char b[100];
	int flag;
	int pos;

	printf("请输入主串: ", a);
	gets(a);
	printf("请输入模式串: ");
	gets(b);
	printf("请输入开始匹配位置: ");
	scanf("%d", &pos);

	flag = kmp(a, b, pos-1);
	if (flag >= 0)
		printf("匹配成功!\n位置: %d\n", flag);
	else 
		printf("匹配失败!\n");

	return 0;
}