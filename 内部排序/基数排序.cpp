#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// 链式基数排序, 类似多次使用桶排序

#define NO -1
#define MAX_KEY 10		// 关键字最大长度
#define SIZE 100		// 链表长度
#define MAX 10			// 基的大小
// 这里只实验十进制数字, 所以是 10, 英文则改为 26

// 单个结点
typedef struct
{
	char data[MAX_KEY];		// 关键字
	int len;		// 串长
	int next;		// 下一个结点
}node;

// 整个静态链表
typedef struct
{
	node link[SIZE];
	int max_len;		// 最大
	int size;
}linklist;


// 初始化链表
void init(linklist *list, int *data, int n)
{
	int i, j = 1;

	list->size = SIZE;   list->max_len = -1;
	for (i=0; i<n; i++)
	{	// 整型转为字符串型
		itoa(data[i], list->link[i].data, 10);
		list->link[i].next = i + 1;
		list->link[i].len = strlen(list->link[i].data);
		if (list->link[i].len > list->max_len) 
			list->max_len = list->link[i].len;
		// max_key 将确定进行多少次基数排序
	}
	list->link[i-1].next = NO;
}

// 基数排序, 对每一位字符采用类似桶排序的办法
void base_sort(int *data, int n)
{
	int head = 0;
	linklist list;		// 0 作为头地址
	int base[MAX];		// 基
	int rear[MAX];		// 尾
	int i, j, p;
	int add, f = 1, last;

	init(&list, data, n);	// 将 data 的数据复制到链表
	for (j=0; j<list.max_len; j++)		// 进行 n 次分摊回收
	{
		// 分摊
		for (i=0; i<MAX; i++) base[i] = rear[i] = NO;
		p = head;
		while (p != NO)
		{
			if (list.link[p].len != 0) 
			{
				list.link[p].len--;
				add = list.link[p].data[ list.link[p].len ] - '0';
			}
			else add = 0;

			if (base[add] != NO)   // 类似链表的插入
			{
				list.link[rear[add]].next = p;
				rear[add] = p;
			}
			else base[add] = rear[add] = p;
			p = list.link[p].next;
		}
		// 回收, 记得 f 每次都要置为 1
		for (f=1, i=0; i<MAX; i++)
		{
			if (base[i] != NO)
			{
				// 第一次, 记录头结点地址 base[i]
				// 不是第一次, 则把上一次的末尾, 连到这次的头 base[i]
				if (f) {head = base[i];   f = 0;}
				else list.link[last].next = base[i];
				last = rear[i];
			}
		}
		list.link[last].next = NO;
	}
	// 将链表的数据写回 data 中
	i = 0;
	while (head != NO)
	{
		data[i++] = atoi(list.link[head].data);
		head = list.link[head].next;
	}
}

int main(void)
{
	int i, n;
	int data[100];

	printf("请输入 n, 及 n 个元素: ");
	scanf("%d", &n);
	for (i=0; i<n; i++) scanf("%d", data + i);

	base_sort(data, n);
	printf("排序后: ");
	for (i=0; i<n; i++) printf("%d, ", data[i]);
	printf("\n\n");
	return 0;
}
