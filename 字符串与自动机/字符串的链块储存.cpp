#include <stdio.h>
#include <malloc.h>

// 串的链式储存, 每个结点放 6 个元素
#define OK 1
#define ERROR 0
#define SIZE 6

typedef int status;

// 结点结构，放 6 个数据
typedef struct node
{
	char data[SIZE];
	node *next;
}node;

// 头结构
typedef struct 
{
	node *first;
	node *last;
	int lenth;	//  串长
}string;

// 建立一个空结点起连接作用，不放数据
status create(string *s)
{
	// [NULL] -> [1] -> [2]...
	s->first = (node *)malloc(sizeof(node));
	if (!s->first) return ERROR;
	s->first->next = NULL;	// 安全性
	s->last = s->first;
	s->lenth = 0;

	return OK;
}

// 追加一个新结点
status append(string *s)
{
	node *n;

	n = (node *)malloc(sizeof(node));
	if (!n) return ERROR;
	n->next = s->last->next;
	s->last->next = n;	// 把 n 连上去
	s->last = n;		// last 自增

	return OK;
}

// 清空字符串
status strclear(string *s)
{
	node *cur;

	if (!s->first) return ERROR;
	cur = s->first;
	while (cur->next)
	{
		s->first = cur->next;
		free(cur);
		cur = s->first;
	}
	s->last = s->first;
	s->lenth = 0;
	return OK;
}

// 输入数据，相当于 scanf
status input(string *s)
{
	char buf[1024] = {0};	// 接收缓冲区
	char *cur;
	int len = 0;
	int i;
	char *in;

	strclear(s);
	scanf("%s", buf);
	while (buf[len] != '\0') len++;
	cur = buf;
	// 计算好用户输入字符串的长度，开始写入
	while (len > 0)
	{
		// 末尾追加一个新结点，
		if (!append(s)) return ERROR;

		// 一次写入 SIZE 个
		// in 指向这个新结点，开始赋值
		in = s->last->data;
		for (i=0; i<SIZE; i++)
		{
			if (cur[i] != '\0') 
			{
				in[i] = cur[i];
				s->lenth++;
			}
			else 
			{
				for ( ; i<SIZE; i++) in[i] = '#';
				break;
			}
		}
		// cur 去到下一个 6 
		cur += SIZE;
		len -= SIZE;
	}
	return OK;
}

// 遍历链式字符串，并输出
status strshow(string *s)
{
	node *cur;
	int times;
	char *c;

	if (!s->first) return ERROR;
	cur = s->first->next;
	while (cur)
	{
		times = SIZE;
		c = cur->data;
		while (times--)
		{
			if (*c != '#') printf("%c", *c++);
		}
		cur = cur->next;
	}
	printf("\n");
	return OK;
}

// 最省事的方法，直接把 a 中的结点全部
// 复制到 b 中就行，
// 但是 a 中可能存在很多的 # 无效符，
// 为了省空间，重新遍历 a 取有用元素
status strcopy(string *b, string *a)
{
	node *cur;
	int now;	// b中指示器，在0到5之间循环
	int i;

	// 先释放掉 b 原来的，
	strclear(b);
	// 遍历 a 复制到 b 中
	cur = a->first->next;
	now = SIZE;
	while (cur)
	{
		// 遍历 cur 结点的数据倒入 b 中
		for (i=0; i<SIZE; i++)
		{
			// 如果当前的结点满了
			if (now == SIZE)
			{
				if (!append(b)) return ERROR;
				now = 0;
			}
			// 开始倒入到 b 中
			if (cur->data[i] != '#')
			{
				b->last->data[now++] = cur->data[i];
				b->lenth++;
			}
		}
		cur = cur->next;	// 复制下一个结点数据
	}
	// b 的最后一个结点不一定放满
	while (now != SIZE) b->last->data[now++] = '#';

	return OK;
}

// 为了不破坏 s 的物理结构，
// 所以重新复制一份到 re
status strcatch(string *re, string *s)
{
	node *cur;
	int i;

	if (!re->first) return ERROR;
	// 遍历 s
	cur = s->first->next; 
	while (cur)
	{
		// 在 re 中追加一个新结点
		if (!append(re)) return ERROR;
		for (i=0; i<SIZE; i++)
		{
			re->last->data[i] = cur->data[i];
		}
		cur = cur->next;
	}
	// 更新长度
	re->lenth += s->lenth;
	return ERROR;
}

// 在 re 中截取子串到 s 中
// 直接在
status strback(string *re, int pos, int len, string *s)
{
	int j = 0;
	node *cur;
	int i;
	char buf[1024];
	int now = 0;
	int position;

	// abcd
	if (pos >= 1 
		&& len >= 0 
		&& pos + len <= re->lenth + 1)
	{
		strclear(s);
		cur = re->first->next;
		position = pos + len - 1;
		while (cur)
		{
			// 遍历每个结点的数据
			// 取出数据放到 buf 中
			for (i=0; i<SIZE; i++)
			{
				if (cur->data[i] != '#') 
				{
					buf[now++] = cur->data[i];	
					if (now == position) break;
				}
			}
			cur = cur->next;
		}
		// 把 buf 中的数据倒入 s 中
		now = SIZE;
		for (i=pos-1; i<position; i++)
		{
			if (now == SIZE)
			{
				if (!append(s)) return ERROR;
				now = 0;
			}
			s->last->data[now++] = buf[i];
		}
		s->lenth = len;
		while (now != SIZE) s->last->data[now++] = '#';
		return OK;

	}
	else return ERROR;
}

int strcompare(string *a, string *b)
{
	node *cura = a->first->next;	// 记录串中的位置
	node *curb = b->first->next;
	int posa = 0;
	int posb = 0;
	char aa, bb;

	if (a->lenth != 0 && b->lenth != 0)
	{
		while (cura && curb)
		{
			// 寻找 a 中最近一个元素
			aa = bb = 0;
			while (1)
			{
				if (cura->data[posa] != '#')
				{
					aa = cura->data[posa];
					posa++;
					break;
				}
				else 
				{
					posa++;
					if (posa == SIZE)
					{
						posa = 0;
						cura = cura->next;
						if (!cura) break;
					}
				}
			}
			// 寻找 b 中最近一个元素
			while (1)
			{
				if (curb->data[posb] != '#')
				{
					bb = curb->data[posb];
					posb++;
					break;
				}
				else 
				{
					posb++;
					if (posb == SIZE)
					{
						posb = 0;
						curb = curb->next;
						if (!curb) break;
					}
				}
			}
			if (aa && bb) 
			{
				if (aa != bb) return aa - bb;
			}
			else return aa - bb;
		}
	}
	
	// 如果有一个是 0 就不执行循环
	return a->lenth - b->lenth;
}

status destory(string *s)
{
	node *cur;

	cur = s->first;
	while (cur)
	{
		s->first = cur->next;
		free(cur);
		cur = s->first;
	}
	s->last = s->first;
	s->lenth = 0;

	return OK;
}

int main(void)
{
	string s1, s2, s3;
	int a, b;

	create(&s1);	// 初始化
	create(&s2);
	create(&s3);

	input(&s1);	// 串复制
	strshow(&s1);
	printf("长度：%d\n", s1.lenth);

	strcopy(&s2, &s1);	// 串遍历
	strshow(&s2);

	input(&s3);
	strcatch(&s2, &s3);	// 串连接
	strshow(&s2);	
	printf("长度：%d\n", s2.lenth);

	printf("截取：位置+长度:");
	scanf("%d%d", &a, &b);
	// 从第 1 位，截取 3 个
	strback(&s1, a, b, &s2);	// 串截取
	strshow(&s2);

	// 串比较
	input(&s2);
	if (strcompare(&s1, &s2) > 0) printf("s1 > s2\n");
	else if (strcompare(&s1, &s2) < 0) printf("s1 < s2\n");
	else if (strcompare(&s1, &s2) == 0) printf("s1 = s2\n");

	strclear(&s1);	// 清空
	strclear(&s2);
	strshow(&s2);

	destory(&s1);
	destory(&s2);
	destory(&s3);

	return 0;
}