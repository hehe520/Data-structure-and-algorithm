#include <stdio.h>
#include <stdlib.h>

typedef int status;
typedef int ElemType;

#define OK         1
#define ERROR      0
#define OVERFLOW  -2
#define SIZE      50  // 原大小
#define INS       10  // 增量

typedef struct
{
	ElemType *elem;
	int length;
	int size;
}sqlist;

// elem指向一块空间，里面有我要的数据
// *p.elem 就可以取出我的数据

status init_sq(sqlist *p)  // 创建sqlist表
{
	p->elem = (ElemType *)malloc(SIZE * sizeof(ElemType));
	// 建100个存储空间
	if (p)
	{
		p->length = 0;
		p->size = SIZE;
		return OK;
	}
	else return ERROR;
}

status destory_sq(sqlist *p)  // 销毁
{
	if (p->size)
	{
		free(p->elem);
		//不能把 p 都释放掉了，p是auto型，由编译器释放
		// p->elem 才是可以释放的
		return OK;
	}
	else return ERROR;
}

// 插入数据
status listinsert(sqlist *p, int i, ElemType data)
{
	ElemType *newbase;
	ElemType *pos;
	ElemType *cur;

	// i 的位置是否正确
	if (i>=0 && i<=(p->length))
	{
		// 表是不是满了
		if (p->length == p->size)
		{
			newbase = (ElemType *)realloc(p->elem, (p->size + INS)*sizeof(ElemType));
			if (newbase)
			{
				p->elem = newbase;
				p->size += INS;
			}
			else return OVERFLOW;
		}
		// 开始插入
		pos = p->elem + i; // 待插入的位置
		for (cur = p->elem + p->length; cur>=pos; cur--)
		{
			// cur 指向 p.elem[length] 最后一个元素的地址
			// *cur 就是取p.elem[length] 指向一块空间的数据
			*(cur+1) = *cur;
		}
		*pos = data;
		p->length++;
		return OK;
	}
	else return ERROR;
}


// 删除数据
status listdelete(sqlist *p, int i)
{
	ElemType *pos;
	ElemType *cur;
	ElemType *end;

	if (i>=0 && i<(p->length))
	{
		// a[i]  []有寻址+取值的作用
		// 用 *(a + i) 也是可以的
		pos = &(p->elem[i]);  // 待删除的数据
		end = p->elem + p->length;   // 尾地址
		for (cur=pos; cur<=end; cur++)
		{
			*cur = *(cur+1);
		}
		p->length--;
		return OK;
	}
	else return ERROR;
}


// 清空表
status listclear(sqlist *p)
{
	ElemType *cur;
	ElemType *end;

	if (p->length > 0)
	{
		end = p->elem + p->length;
		for (cur=p->elem; cur<end; cur++)
		{
			*cur = 0;
		}
		return OK;
	}
	else return ERROR;
}

sqlist *show(sqlist *p)  // 遍历表中所有元素，
{
	ElemType *cur;
	ElemType *end;
	int i = 0;

	printf("表中元素：");
	end = p->elem + p->length;
	for (cur=p->elem; cur<end; cur++)
	{
		if (i%15 == 0) printf("\n");
		i++;
		printf("%4d", *cur);
	}
	printf("\n");

	return p;
}

// 获得第 i 的元素的值
status getelem(sqlist *p, int i, ElemType *back)
{
	if (i>=0 && i<=(p->length))
	{
		*back = *(p->elem + i);
		return OK;
	}
	else return ERROR;
}

// 获得表长
status listlength(sqlist *p)
{
	return p->length;
}

int main(void)
{
	sqlist list;
	int i;
	ElemType data;

	init_sq(&list);
	for (i=0; i<10; i++)
	{
		listinsert(&list, i, i);
	}		
	show(&list);
	printf("表长 %d\n", listlength(&list));

	printf("请输入要插入数据，及位置：");
	scanf("%d %d", &data, &i);
	if (listinsert(&list, i-1, data)) printf("插入成功!\n\n");
	else printf("插入失败!\n\n");
	show(&list);

	printf("请输入要删除数据的位置：");
	scanf("%d",&i);
	if (listdelete(&list, i-1)) printf("删除成功!\n\n");
	else printf("删除失败!\n\n");
	show(&list);

	printf("输入访问元素的位置：");
	scanf("%d",&i);
	if (getelem(&list, i-1, &data)) 
	{
		printf("找到！该元素为 %d\n", data);
	}
	else printf("没有找到！\n");

	printf("\n将表重置为 0\n");
	if (listclear(&list)) printf("重置成功！\n");
	else printf("重置失败!\n");
	show(&list);

	destory_sq(&list);

	return 0;
}
