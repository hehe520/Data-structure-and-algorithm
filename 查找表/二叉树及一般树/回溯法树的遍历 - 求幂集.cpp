#include <stdio.h>
#include <stdlib.h>

// 回溯法, 遍历状态树, 求 n 个元素的幂集

typedef int status;
typedef char elemtype;
#define ERROR 0
#define OK 1

typedef struct		// 顺序表表示集合
{
	elemtype *base;
	int lenth;
	int size;
}sqlist;

status create(sqlist *list)
{
	list->base = (elemtype *)malloc(20 * sizeof(elemtype));
	if (!list->base) return ERROR;
	list->lenth = 0;
	list->size = 20;
	return 1;
}

status insert(sqlist *list, int pos, elemtype *data)
{
	elemtype *n;
	elemtype *end;
	elemtype *st;
	elemtype *cur;

	if (pos > list->lenth + 1) return ERROR;
	if (list->lenth == list->size)
	{
		n = (elemtype *)realloc(list->base, 
			(10 + list->size) * sizeof(elemtype));
		if (!n) return ERROR;
		list->base = n;
		list->size += 10;
	}
	end = list->base + list->lenth - 1;
	st = list->base + pos - 1;
	for (cur=end; cur>=st; cur--)
		*(cur + 1) = *cur;		// 不断后移
	*st = *data;
	list->lenth++;
	return OK;
}

status del(sqlist *list, int pos)
{
	elemtype *end;
	elemtype *st;
	elemtype *cur;

	if (pos > list->lenth) return ERROR;
	end = list->base + list->lenth - 1;
	st = list->base + pos - 1;
	for (cur=st; cur<=end; cur++)
		*cur = *(cur + 1);		// 不断前移
	list->lenth--;
	return OK;
}

void destory(sqlist *list)
{
	if (list->base) free(list->base);
	list->base = NULL;
	list->lenth = 0;
	list->size = 0;
}

void show(sqlist *list)
{
	int i;

	if (list->lenth == 0) printf("{@}\n");   // 空集的表示
	else 
	{
		printf("{");
		for (i=0; i<list->lenth; i++)
		{
			printf("%c", list->base[i]);
			if (i+1 != list->lenth) printf(", ");
		}
		printf("}\n");
	}
}

void input(sqlist *list)
{
	int n, i;
	char ch;

	scanf("%d", &n);
	fflush(stdin);
	for (i=0; i<n; i++)
	{
		scanf("%c", &ch);
		insert(list, i+1, &ch);
	}
	fflush(stdin);
}

// 求幂集的递归部分, i 表示深度
int set_s(sqlist *list, sqlist *b, int i)
{
	int num = 0;
	char ch;
	int len;

	if (i > list->lenth)	// 到达了最底层,找到解
	{
		show(b);
		return 1;
	}
	ch = list->base[i - 1];   // 获得一个元素
	len = b->lenth + 1;		// 获得插入的位置

	// 类似二叉树的遍历
	insert(b, len, &ch);	// 放入 b 中
	num += set_s(list, b, i + 1);

	// 回溯时, 删去ch, 同时又产生新的递归
	del(b, len);			// 不放入 b 中
	num += set_s(list, b, i + 1);

	return num;
}

// 遍历深度为 n 的 n 叉树
// 叶子结点正好是解, 
int set(sqlist *list)
{
	sqlist tmp;	// 临时集合
	int num;

	create(&tmp);
	num = set_s(list, &tmp, 1);
	destory(&tmp);
	return num;
}


int main(void)
{
	sqlist list;
	sqlist b;
	int num;

	create(&list);   create(&b);
	printf("请输入字符集合: ");
	// 输入方式: 开头一个 n, 随后 n 个字符集
	input(&list);
//	show(&list);

	num = set(&list);	// 求 list 的幂集
	printf("\nlist 的幂集有 %d 个\n", num);

	destory(&list);
	return 0;
}
