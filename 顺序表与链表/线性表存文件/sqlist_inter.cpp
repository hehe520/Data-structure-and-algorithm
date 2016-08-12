#include <stdio.h>
#include <malloc.h>
#include "sqlist.h"

// --------list_inter主要函数--------
status createlist(sqlist *p, int size)
{
	p->base = (elemtype *)malloc(size*sizeof(elemtype));
	if (!p->base) return ERROR;
	p->lenth = 0;
	p->size = size;

	return OK;
}

status destorylist(sqlist *p)
{
	if (p->base)
	{
		free(p->base);
		p->base = NULL;
		p->lenth = 0;
		p->size = 0;
		return OK;
	}
	else return ERROR;
}

status insertlist(sqlist *p, int pos, elemtype *data)
{
	elemtype *n;
	elemtype *end;
	elemtype *now;
	elemtype *cur;
	int size;

	if (!p->base) return ERROR;
	if (pos>=1 && pos<=p->lenth + 1)
	{
		if (p->lenth == p->size)
		{
			size = p->size + INC;
			n = (elemtype *)realloc(p->base, size*sizeof(elemtype));
			if (!n) return ERROR;
			p->base = n;
			p->size += SIZE;
		}
		// 1 2 3  lenth==3
		end = p->base + p->lenth - 1;
		now = p->base + pos - 1;
		for (cur=end; cur>=now; cur--)
		{
			*(cur + 1) = *cur;
		}
		*now = *data;
		p->lenth++;

		return OK;
	}
	else return ERROR;
}

status showlist(sqlist *p)
{
	elemtype *cur;
	elemtype *end;

	if (!p->base) return ERROR;
	else 
	{
		end = p->base + p->lenth - 1;
		for (cur=p->base; cur<=end; cur++)
		{
			printf("%d, ", *cur);
		}
		printf("\n");
		return OK;
	}
}

status existfile(const char *path)
{
	FILE *p;

	p = fopen(path, "rb");
	if (p)
	{
		fclose(p);
		return OK;
	}
	else return ERROR;
}

// 把 list 写到 path , 覆盖模式
status writefile(sqlist *p, const char *path)
{
	FILE *out;

	// 覆盖模式，先清除原来的
	if (existfile(path)) remove(path);
	out = fopen(path, "wb");
	if (out)
	{
		// fwrite每次最多只能写65535*65535字节
		fwrite(p->base, sizeof(elemtype), p->lenth, out);
		fclose(out);
		return OK;
	}
	else return ERROR;
}

// 从 path 中读取文件 覆盖到 list 中
status readfile(sqlist *p, const char *path)
{
	FILE *in;
	int size;
	int an;

	in = fopen(path, "rb");
	if (in)
	{	
		// 检查文件大小
		fseek(in, 0L, SEEK_END);
		size = ftell(in);
		rewind(in);

		// 根据文件大小来开表的大小
		createlist(p, size);

		an = fread(p->base, 1, size, in);
		p->size = p->lenth = an / sizeof(elemtype);
		return OK;
	}
	else return ERROR;
}

// --------node_inter主要函数--------
status createnode(head *p)
{
	node *n;

	// 第一个结点不存元素，起连接作用
	// [NULL] -> [1] -> [0] ->..
	n = (node *)malloc(sizeof(node));
	if (!n) return ERROR;
	n->next = NULL;

	p->first = p->end = n;
	p->lenth = 0;

	return OK;
}

status destorynode(head *p)
{
	node *cur;

	cur = p->first;
	while (cur)
	{
		p->first = cur->next;
		free(cur);
		cur = p->first;
	}
	p->first = p->end = NULL;
	p->lenth = 0;
	return OK;
}

status shownode(head *p)
{
	node *cur;

	cur = p->first->next;

	while (cur)
	{
		printf("%d, ", cur->data);
		cur = cur->next;
	}
	return OK;
}

// 追加到最后
status append(head *p, elemtype *data)
{
	node *n;

	if (p->first && p->end)
	{
		n = (node *)malloc(sizeof(node));
		if (!n) return ERROR;
		n->data = *data;
		// 接到最后面
		n->next = p->end->next;
		p->end->next = n;
		p->end = n;		// 相当于 end 后移一步
		
		return OK;
	}
	else return ERROR;
}

// 把链表写到path里面
status writefile2(head *p, const char *path)
{
	node *cur;
	FILE *out;

	// 打开文件
	if (existfile(path)) remove(path);
	out = fopen(path, "wb");
	if (out)
	{
		cur = p->first->next;
		while (cur)
		{
			// 去除指针信息，直接将数据写进去
			fwrite(&cur->data, sizeof(elemtype), 1, out);
			cur = cur->next;
		}
		fclose(out);
		return OK;
	}
	else return ERROR;
}

// 从 path 中读取数据
status readfile2(head *p, const char *path)
{
	FILE *in;
	elemtype data;
	int an;

	in = fopen(path, "rb");
	if (in)
	{
		// 每次读取一个数据
		while (1)
		{
			an = fread(&data, sizeof(elemtype), 1, in);
			if (!an) break;
			append(p, &data);
		}
	}
	else return ERROR;

	return OK;
}

