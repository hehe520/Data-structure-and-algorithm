// 存放顺序表的相关函数
#include <stdio.h>
#include "sqlist.h"
#include <malloc.h>

void init(sqlist *p)	// 初始化表
{
	p->elem = NULL;
	p->length = 0;
	p->size = 0;
}

status createlist(sqlist *p)	// 创建顺序表
{
	elemtype *newbase = NULL;
	
	if (p->elem) return ERROR;	// 已经创建了
	newbase = (elemtype *)malloc(SIZE*sizeof(elemtype));
	if (newbase)
	{
		p->elem = newbase;
		p->length = 0;
		p->size = SIZE;
		return OK;
	}
	else return ERROR;
}

status destorylist(sqlist *p)	// 销毁线性表
{
	if (p->elem)
	{
		free(p->elem);
		p->length = 0;
		p->size = 0;
		p->elem = NULL;
		return OK;
	}
	else return ERROR;
}

status insert(sqlist *p, int pos, elemtype *data) // 插入数据
{
	elemtype *end;
	elemtype *cur;
	elemtype *now;
	elemtype *newbase = NULL;
	int newsize = 0;

	if (p->elem == NULL) return ERROR; 
	// pos位置是否正确
	// 下标从 0 开始，而位序从 1 开始，所以pos--
	pos--;
	if (pos>=0 && pos<=p->length)
	{
		// 看看表的大小够不够
		if (p->length == p->size)
		{
			newsize = (p->size + INC) * sizeof(elemtype);
			newbase = (elemtype *)realloc(p->elem, newsize);
			if (newbase)
			{
				p->elem = newbase;
				p->size += INC;
			}
			else return ERROR;
		}

		// 开始插入
		end = p->elem + p->length - 1; 
		now = p->elem + pos;
		for (cur=end; cur>=now; cur--)
		{
			*(cur+1) = *cur;
		}
		*now = *data;
		p->length++;
		return OK;
	}
	else return ERROR;
}

status update(sqlist *p, int pos, elemtype *data) // 修改数据
{
	elemtype *cur;

	if (p->elem == NULL) return ERROR; 
	pos--;	// 统一下标与位序
	if (pos>=0 && pos<p->length)
	{
		cur = p->elem + pos;
		*cur = *data;
		return OK;
	}
	else return ERROR;
}

status remove(sqlist *p, int pos) // 删除数据
{
	elemtype *cur;
	elemtype *end;

	if (p->elem == NULL) return ERROR; 
	pos--;	// 统一下标与位序
	if (pos>=0 && pos<p->length)
	{	
		end = p->elem + p->length - 1;
		for (cur=p->elem + pos; cur<end; cur++)
		{
			*cur = *(cur + 1);
		}
		p->length--;
		return OK;
	}
	else return ERROR;
}

status showlist(sqlist *p) // 显示表中所有元素
{
	elemtype *cur;
	int i;

	if (p->length)
	{
		// 每一行显示10个
		cur = p->elem;
		for (i=0; i<p->length; i++)
		{
			if (i%10 == 0) printf("\n\n");
			printf("%d, ", *cur);
			cur++;
		}
		return OK;
	}
	else return ERROR;
}

status clearlist(sqlist *p) // 清空表中元素
{
	elemtype *cur;
	elemtype *end;

	if (p->length)
	{
		// 1 2 3  lenth = 3
		end = p->elem + p->length;
		for (cur=p->elem; cur<end; cur++)
		{
			*cur = 0;
		}
		return OK;
	}
	else return ERROR;
}

status getelem(sqlist *p, int pos, elemtype *back) // 访问第N个元素
{
	elemtype *cur;

	if (p->elem == NULL) return ERROR; 
	pos--;
	if (pos>=0 && pos<p->length)
	{	
		cur = p->elem + pos;
		*back = *cur;
		return OK;
	}
	else return ERROR;
}

status listlen(sqlist *p, int *back) // 获得表长
{
	if (p->elem == NULL) return ERROR; 
	*back = p->length;
	return OK;
}

