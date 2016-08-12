#include <stdio.h>
#include <stdlib.h>

// 堆排序, 利用二叉堆进行排序

#define SIZE 102	// 堆大小
#define MIN -0XFFFFFFF
#define MAX 0XFFFFFFF

typedef struct		// 二叉堆储存
{
	int *base;
	int lenth;
}heap;

void create(heap *h, int flag)		// 建立堆
{
	h->base = (int *)malloc(SIZE * sizeof(int));
	if (flag) h->base[0] = MAX;
	else h->base[0] = MIN;
	h->lenth = 0;
}

void destory(heap *h)		// 销毁堆
{
	if (h && h->base) free(h->base);
	h->lenth = 0;
}

void node_up_s(heap *h, int p)	// 上调小顶堆
{
	int key = h->base[p];	// 记录待调结点

	while (key < h->base[p / 2])
	{
		h->base[p] = h->base[p / 2];
		p /= 2;
	}
	h->base[p] = key;
}

void node_up_l(heap *h, int p)	// 上调大顶堆
{
	int key = h->base[p];	// 记录待调结点

	while (key > h->base[p / 2])
	{
		h->base[p] = h->base[p / 2];
		p /= 2;
	}
	h->base[p] = key;
}

void node_down_s(heap *h, int p)	// 下调小顶堆
{
	int lc, rc;
	int key = h->base[p];

	while (1)
	{
		lc = 2 * p;   rc = lc + 1;
		if (lc > h->lenth) break;
		if (rc > h->lenth)
		{
			// 只有左孩子
			if (h->base[lc] < key)
			{
				h->base[p] = h->base[lc];
				p = lc;
			}
			else break;		// 不用交换了, 要及时退出
		}
		else	// 左右都有
		{
			if (h->base[lc] < h->base[rc])
			{
				if (h->base[lc] < key)
				{
					h->base[p] = h->base[lc];
					p = lc;
				}
				else break;
			}
			else 
			{
				if (h->base[rc] < key)
				{
					h->base[p] = h->base[rc];
					p = rc;
				}
				else break;
			}
		}
	}
	h->base[p] = key;	// 别忘记插回去
}

void node_down_l(heap *h, int p)	// 下调大顶堆
{
	int lc, rc;
	int key = h->base[p];

	while (1)
	{
		lc = 2 * p;   rc = lc + 1;
		if (lc > h->lenth) break;
		if (rc > h->lenth)
		{
			if (h->base[lc] > key)
			{
				h->base[p] = h->base[lc];
				p = lc;
			}
			else break;
		}
		else 
		{	// 和小顶堆相反, 孩子中最大的上浮
			if (h->base[lc] > h->base[rc])
			{
				if (h->base[lc] > key)
				{
					h->base[p] = h->base[lc];
					p = lc;
				}
				else break;
			}
			else 
			{
				if (h->base[rc] > key)
				{
					h->base[p] = h->base[rc];
					p = rc;
				}
				else break;
			}
		}
	}
	h->base[p] = key;	// 别忘记插回去
}

// 将堆顶出队, 并且删除堆顶
int pop_del(heap *h, int flag)
{
	int result = h->base[1];		// 记录堆顶

	// 将最后一个元素, 放到堆顶, 然后再调整堆顶
	h->base[1] = h->base[h->lenth];
	h->lenth--;
	if (flag) node_down_l(h, 1);	// 按大顶堆来调
	else node_down_s(h, 1);

	return result;
}

// 插入堆
void insert(heap *h, int data, int flag)
{
	h->lenth++;
	h->base[h->lenth] = data;
	if (flag) node_up_l(h, h->lenth);	// 按大顶堆来调
	else node_up_s(h, h->lenth);		// 按小顶堆来调
}

// 小顶堆, 升序排序
void heap_sort_up(int *data, int n)
{
	heap h;
	int i;

	create(&h, 0);		// 0 是建立小顶堆
	for (i=0; i<n; i++) insert(&h, data[i], 0);
	for (i=0; i<n; i++) data[i] = pop_del(&h, 0);
	destory(&h);
}

// 大顶堆, 降序排序, 全部元素进入堆 (优先队列) 中
// 再全部出来就是有序的了
void heap_sort_down(int *data, int n)
{
	heap h;
	int i;

	create(&h, 1);		// 1 是建立大顶堆
	for (i=0; i<n; i++) insert(&h, data[i], 1);
	for (i=0; i<n; i++) data[i] = pop_del(&h, 1);
	destory(&h);
}

int main(void)
{
	int i, n;
	int data[100];
	int data2[100];

	printf("请输入 n, 及 n 个元素: ");
	scanf("%d", &n);
	for (i=0; i<n; i++)
	{
		scanf("%d", data + i);
//		data[i] = rand() % 100;	// (测试用)
		data2[i] = data[i];		// 再复制多一份
	}
	heap_sort_up(data, n);
	printf("升序: ");
	for (i=0; i<n; i++) printf("%d, ", data[i]);
	printf("\n\n");
	
	heap_sort_down(data2, n);
	printf("降序: ");
	for (i=0; i<n; i++) printf("%d, ", data2[i]);
	printf("\n\n");

	return 0;
}
