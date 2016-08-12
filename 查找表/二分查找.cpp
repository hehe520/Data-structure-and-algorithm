#include <stdio.h>
#include <malloc.h>

#define NOT_FOUND -1
#define OK 1
#define ERROR 0
#define SIZE 30
typedef int status;


// 关键字结点结构
typedef struct
{
	int key;
	char *info;
}keytype;

// keytype 顺序表
typedef struct
{
	keytype *elem;
	int lenth;
	int size;
}table;

status create(table *t)
{
	t->lenth = 0;
	t->size = SIZE;
	t->elem = (keytype *)malloc(SIZE * sizeof(keytype));
	if (!t->elem) return ERROR;
	return OK;
}

status destory(table *t)
{
	if (t) free(t->elem);
	t->elem = NULL;
	t->lenth = t->size = 0;
	return OK;
}

status append(table *t, keytype *data)
{
	// 追加空间不是重点，所以不追加了
	if (t->lenth == t->size) return ERROR;

	t->elem[t->lenth].key = data->key;
	t->lenth++;
	return OK;
}

void show(table *t)
{
	int i;
	if (t)
		for (i=0; i<t->lenth; i++)
			printf("%d, ", t->elem[i].key);
}

// 二分查找函数, flag == 1 升序，flag == 0 降序
status bin_search(table *t, keytype *data, short int flag)
{
	int left = 0;
	int right = t->lenth - 1;
	int mid;

	if (flag)   // 升序序列
	{
		while (left <= right)
		{
			mid = (left + right) / 2;
			if (t->elem[mid].key == data->key)
				return mid;
			if (t->elem[mid].key < data->key)
				left = mid + 1;
			if (t->elem[mid].key > data->key)
				right = mid - 1;
		}
	}
	else    // 降序序列
	{
		while (left <= right)
		{
			mid = (left + right) / 2;
			if (t->elem[mid].key == data->key)
				return mid;
			if (t->elem[mid].key < data->key)
				right = mid - 1;
			if (t->elem[mid].key > data->key)
				left = mid + 1;
		}
	}
	return NOT_FOUND;
}

int main(void)
{
	table t;
	keytype tmp;
	int pos;

	create(&t);
	printf("请按升序输入关键字：");
	while (scanf("%d", &tmp.key))
		append(&t, &tmp);
	show(&t);
pre:
	printf("\n请输入待查找的元素: ");
	fflush(stdin);
	scanf("%d", &tmp.key);
	pos = bin_search(&t, &tmp, 1);		// 二分查找函数
	if (pos != NOT_FOUND)
		printf("找到：位序：%d, 关键字：%d\n", pos+1, tmp.key);
	else printf("未找到\n");
	goto pre;
	destory(&t);

	return 0;
}