#include <stdio.h>
#include <stdlib.h>

// 堆排序, 方法二, 直接在 data 上面调整, 直到符合堆性质
// 即将任意一个数组, 调整为堆

#define MIN -0XFFFFFFF
#define MAX 0XFFFFFFF

void node_up_s(int *data, int p)	// 上调小顶堆
{
	int key = data[p];	// 记录待调结点

	while (key < data[p / 2])
	{
		data[p] = data[p / 2];
		p /= 2;
	}
	data[p] = key;
}

void node_up_l(int *data, int p)	// 上调大顶堆
{
	int key = data[p];	// 记录待调结点

	while (key > data[p / 2])
	{
		data[p] = data[p / 2];
		p /= 2;
	}
	data[p] = key;
}

void node_down_s(int *data, int *len, int p)	// 下调小顶堆
{
	int lc, rc;
	int key = data[p];

	while (1)
	{
		lc = 2 * p;   rc = lc + 1;
		if (lc > *len) break;
		if (rc > *len)
		{
			// 只有左孩子
			if (data[lc] < key)
			{
				data[p] = data[lc];
				p = lc;
			}
			else break;		// 不用交换了, 要及时退出
		}
		else	// 左右都有
		{
			if (data[lc] < data[rc])
			{
				if (data[lc] < key)
				{
					data[p] = data[lc];
					p = lc;
				}
				else break;
			}
			else 
			{
				if (data[rc] < key)
				{
					data[p] = data[rc];
					p = rc;
				}
				else break;
			}
		}
	}
	data[p] = key;	// 别忘记插回去
}

void node_down_l(int *data, int *len, int p)	// 下调大顶堆
{
	int lc, rc;
	int key = data[p];

	while (1)
	{
		lc = 2 * p;   rc = lc + 1;
		if (lc > *len) break;
		if (rc > *len)
		{
			if (data[lc] > key)
			{
				data[p] = data[lc];
				p = lc;
			}
			else break;
		}
		else 
		{	// 和小顶堆相反, 孩子中最大的上浮
			if (data[lc] > data[rc])
			{
				if (data[lc] > key)
				{
					data[p] = data[lc];
					p = lc;
				}
				else break;
			}
			else 
			{
				if (data[rc] > key)
				{
					data[p] = data[rc];
					p = rc;
				}
				else break;
			}
		}
	}
	data[p] = key;	// 别忘记插回去
}

// 将堆顶出队, 并且删除堆顶
int pop_del(int *data, int *len, int flag)
{
	int result = data[1];		// 记录堆顶

	// 将最后一个元素, 放到堆顶, 然后再调整堆顶
	data[1] = data[*len];
	(*len)--;
	if (flag) node_down_l(data, len, 1);	// 按大顶堆来调
	else node_down_s(data, len, 1);

	return result;
}

// 小顶堆, 升序排序
// 从上到下的把小元素上浮
// 这里有一个巧妙的做法, 不用开辟另外数组来储存出队的元素, 
// 因为出队后, data[末尾] 是空闲的, 所以, 直接放到最后面即可
void heap_sort_up(int *data, int n)
{
	int i, len = n;
	for (i=1; i<=n; i++) node_up_s(data, i);
	for (i=n; i>0; i--) data[i] = pop_del(data, &len, 0);
}

// 大顶堆, 降序排序
void heap_sort_down(int *data, int n)
{
	int i, len = n;
	for (i=1; i<=n; i++) node_up_l(data, i);
	for (i=n; i>0; i--) data[i] = pop_del(data, &len, 1);
}

int main(void)
{
	int i, n;
	int data[100];
	int data2[100];

	data[0] = MIN;   data2[0] = MAX;
	// data[] 0 号单元放无穷, 数据区间 [1..n]
	printf("请输入 n, 及 n 个元素: ");
	scanf("%d", &n);
	for (i=1; i<=n; i++)
	{
		scanf("%d", data + i);
//		data[i] = rand() % 100;	// (测试用)
		data2[i] = data[i];		// 再复制多一份
	}
	heap_sort_down(data2, n);
	printf("升序: ");
	for (i=1; i<=n; i++) printf("%d, ", data2[i]);
	printf("\n\n");

	heap_sort_up(data, n);
	printf("降序: ");
	for (i=1; i<=n; i++) printf("%d, ", data[i]);
	printf("\n\n");

	return 0;
}
