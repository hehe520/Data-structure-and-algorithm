#include <malloc.h>
#include <stdio.h>

typedef int elemtype;
typedef int status;

#define OK    1
#define ERROR 0
#define SIZE  30
#define INC   10

typedef struct
{
	elemtype *elem;
	int length;
	int size;
}sqlist;

status init_sq(sqlist *p)
{
	elemtype *create = NULL;

	create = (elemtype *)malloc(SIZE*sizeof(elemtype));
	if (create)
	{
		p->elem = create;
		p->length = 0;
		p->size = SIZE;
		return OK;
	}
	else return ERROR;
}

status destory_sq(sqlist *p)
{
	if (p->size)
	{
		free(p->elem);
		return OK;
	}
	else return ERROR;
}

status insert(sqlist *p, int i, elemtype *data)
{
	elemtype *newbase = NULL;
	elemtype *cur;
	elemtype *end;
	elemtype *pos;  // 待插入数据的地址

	// i 是否合法
	if (i>=0 && i<=p->length)
	{
		// 大小是否够
		if (p->length == p->size)
		{
			newbase = (elemtype *)realloc(p->elem, (p->size+INC)*sizeof(elemtype));
			if (newbase)
			{
				p->elem = newbase;
				p->size += INC;
			}
			else return ERROR;
		}
		// begin insert
		pos = p->elem + i;
		end = p->elem + p->length;
		for (cur=end; cur>=pos; cur--)
		{
			*(cur+1) = *cur;
		}
		*pos = *data;
		p->length++;
		return OK;
	}
	else return ERROR;
}

status delete_sq(sqlist *p, elemtype i)
{
	elemtype *cur;
	elemtype *end;

	if (i>=0 && i<=p->length)
	{
		end = p->elem + p->length;
		for (cur=p->elem+i; cur<end; cur++)
		{
			*cur = *(cur+1);
		}
		p->length--;
		return OK;
	}
	else return ERROR;
}

void show(sqlist *p)
{
	int i;
	for (i=0; i<p->length; i++)
	{
		printf("%4d", p->elem[i]);
	}
	printf("\n");
}

//  合并两个线性表，
status union_sq(sqlist *p, sqlist *q, sqlist *all)
{
	int len1, len2, len3;
	elemtype *pcur, *qcur;
	elemtype *pend, *qend;
	elemtype *ccur;
	
	len1 = p->length;
	len2 = q->length;
	len3 = len1 + len2;
	
	all->elem = (elemtype *)malloc((len3+1) * sizeof(elemtype));
	if (all)
	{
		all->length = len3;
		all->size = len3;
	}
	else return ERROR;

	if (len1 || len2)
	{
		pcur = p->elem;
		qcur = q->elem;
		ccur = all->elem;
		pend = p->elem + p->length;
		qend = q->elem + q->length;
		while (pcur<pend && qcur<qend)
		{
			if (*pcur <= *qcur) *ccur++ = *pcur++;
			else *ccur++ = *qcur++;
		}
		while (pcur < pend) *ccur++ = *pcur++;
		while (qcur < qend) *ccur++ = *qcur++;
		return OK;
	}
	else return ERROR;
}

// 查找一个元素
status getelem(sqlist *p, elemtype *data, elemtype *back)
{
	int flag = 0;
	elemtype *cur, *end;
	
	end = p->elem + p->length;
	for (cur = p->elem; cur < end; cur++)
	{
		if (*cur == *data) 
		{
			flag = 1;
			*back = *cur;
			break;
		}
	}
	return flag;
}

//  p q 的交集
status set(sqlist *p, sqlist *q, sqlist *all)
{
	int len1, len2, len3;
	elemtype *pcur, *qcur;
	elemtype *pend, *qend;
	elemtype *ccur;
	elemtype back;
	
	len1 = p->length;
	len2 = q->length;
	len3 = len1 + len2;
	
	all->elem = (elemtype *)malloc((len3+1) * sizeof(elemtype));
	if (all) 
	{
		all->length = 0;
		all->size = len3;
	}
	else return ERROR;

	if (len1 || len2)
	{
		pcur = p->elem;
		qcur = q->elem;
		ccur = all->elem;
		pend = p->elem + p->length;
		qend = q->elem + q->length;
		while (pcur < pend) 
		{
			*ccur++ = *pcur++;
			all->length++;
		}
		// 把 p 集合倒入 c 中，
		while (qcur < qend)
		{
			// 在 p 中找 q的元素
			if (getelem(p, qcur, &back) == 0)
			{
				*ccur++ = *qcur;
				all->length++;
			}
			qcur++;
		}
		return OK;
	}
	else return ERROR;
}

int main(void)
{
	sqlist list1;
	sqlist list2;
	sqlist list3;
	sqlist *p = &list1;
	sqlist *q = &list2;
	sqlist *all = &list3;
	int a, b;
	int i;
	elemtype data;

	init_sq(p);
	init_sq(q);
	for (i=0; i<8; i++)
	{
		a = 2*i+1;
		b = 2*i;
		insert(p, i, &a);
		insert(q, i, &b);
	}
	data = 2;
	insert(p, 1, &data);
	data = 4;
	insert(p, 3, &data);

	show(p);
	show(q);
	
//	union_sq(p, q, all);
//	show(all);

	set(p, q, all);
	show(all);

/*	printf("insert a number:");
	scanf("%d %d", &data, &i);
	if (insert(p, i-1, &data)) printf("success!\n");
	else printf("fair\n");
	show(p);

	printf("\ndetele a number:");
	scanf("%d", &i);
	if (delete_sq(p, i-1)) printf("success!\n");
	else printf("fair\n");
	show(p);
*/
	destory_sq(p);
	destory_sq(q);

	return 0;
}
