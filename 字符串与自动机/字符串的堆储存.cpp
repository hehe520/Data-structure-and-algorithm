#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>

typedef int status;

#define OK 1
#define ERROR 0

// 串的堆储存
typedef struct 
{
	char *base;
	int lenth;
}string;

int lenth(char *s)
{
	int len = 0;

	while (*s != '\0') 
	{
		len++;
		s++;
	}
	return len;
}

status strcopy(string *s, char *re)
{
	// 算法，销毁原来的，然后建个新的，再放进来
	char *cur;

	if (lenth(re) <= 0) return ERROR;
	// 如果有，就释放掉
	if (s->base) free(s->base);
	// 重新分配大小
	s->lenth = lenth(re);
	s->base = (char *)malloc((s->lenth+1) * sizeof(char));
	if (!s->base) return ERROR;
	
	cur = s->base;
	while (*re != '\0') *cur++ = *re++;
	*cur = '\0';	// 封口

	return OK;
}

status init(string *s)
{
	s->base = NULL;
	s->lenth = 0;

	return OK;
}

status strshow(string s)
{
	char *cur;

	if (s.lenth <= 0) return ERROR;
	// for (i=0; i<s.lenth; i++)  也是可以的
	for (cur=s.base; *cur!='\0'; cur++)
	{
		printf("%c", *cur);
	}
	printf("\n");

	return OK;
}

status strcompare(string s, string t)
{
	int i;

	for (i=0; i<s.lenth && i<t.lenth; i++)
	{
		if (s.base[i] != t.base[i]) 
			return s.base[i] - t.base[i];
	}
	// 如果有一个串的长度是 0
	return s.lenth - t.lenth;
}

status strclear(string *s)
{
	if (s->base) free(s->base);
	s->lenth = 0;

	return OK;
}

status strback(string s, int pos, int len, string *back)
{
	int i;
	int now = 0;
	int end;

	// 把 back 销毁了，再放新的
	// abcd
	if (pos >= 1 && len >= 0 && pos+len <= s.lenth+1)
	{
		if (back->base) strclear(back);
		back->base = (char *)malloc((len+2)*sizeof(char));
		if (!back->base) return ERROR;
		back->lenth = len;
		
		pos--;	// 位序比下标多了 1
		end = pos + len;
		for (i=pos; i<end; i++)
		{
			back->base[now++] = s.base[i];
		}
		back->base[now] = '\0';

		return OK;
	}
	else return ERROR;
}

status strcatch(string s, string t)
{
	// 算法：原基础上重新扩展 s 的大小，把 t 放进来
	int now = 0;
	int i;

	if (t.lenth <= 0) return ERROR;
	s.lenth += t.lenth;
	s.base = (char *)realloc(s.base, (s.lenth+1)*sizeof(char));
	if (!s.base) exit(0);

	for (i=s.lenth-t.lenth; i<s.lenth; i++)
	{
		s.base[i] = t.base[now++];
	}
	s.base[i] = '\0';

	return OK;
}

int main(void)
{
	string s1, s2;

	init(&s1);	// 初始化
	init(&s2);

	strcopy(&s1, "abcdefg");	// 串复制
	strshow(s1);
	printf("长度：%d\n", s1.lenth);

	strcopy(&s2, s1.base);	// 串遍历
	strshow(s2);	

	strcatch(s2, s1);	// 串连接
	strshow(s2);	

	strback(s1, 4, 4, &s2);	// 串截取
	strshow(s2);

	// 串比较
	strcopy(&s2, "bbcdefg");
	if (strcompare(s1, s2) > 0) printf("s1 > s2\n");
	else if (strcompare(s1, s2) < 0) printf("s1 < s2\n");
	else if (strcompare(s1, s2) == 0) printf("s1 = s2\n");

	strclear(&s1);	// 释放
	strclear(&s2);

	return 0;
}