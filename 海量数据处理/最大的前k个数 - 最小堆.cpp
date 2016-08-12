
// 在海量数据中, 约有一亿个数字 (700 MB), 找出最大的前 k 个数字
// 内存限制 1 MB, 数字不超过 int 范围
// 解法, 将所有数字一个一个入堆, 从堆顶进入, 通常是从堆低进入的
// 并且将堆控制在 k 个元素, 不符合的元素丢弃

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

const int num = 100000000;		// 数字个数
const int k = 30;				// 前 k 大
const int INT_MIN = -2147483647;

int heap[k + 1];	// 最小堆

// 随机生成 num 个数字, 约 500 MB
int random_write(char *path)
{
	FILE *out = NULL;
	int i, j, len;
	char buf[12];

	out = fopen(path, "wt");
	if (out == NULL) return 0;
	srand(time(NULL));
	for (i=0; i<num; i++)
	{
		len = rand() % 9 + 1;
		for (j=0; j<len; j++) 
			buf[j] = rand() % 10 + '0';
		buf[len] = '\n';
		buf[++len] = '\0';
		fwrite(buf, len, 1, out);
	}
	fclose(out);		// 记得要关闭
	return 1;
}

void init_heap(void)		// 初始化最小堆
{
	for (int i=0; i<=k; i++)
		heap[i] = INT_MIN;
}

// 尝试将第 pos 位的元素下调
void node_down(int pos)
{
	int key = heap[pos];
	int left, right;

	while (pos <= k)
	{
		left = 2 * pos;
		right = 2 * pos + 1;
		if (left <= k)
		{
			if (right <= k)		// 左右孩子都有
			{
				if (heap[left] <= heap[right] && key > heap[left])
				{
					heap[pos] = heap[left];
					pos = left;
				}
				else if (heap[left] >= heap[right] && key > heap[right])
				{
					heap[pos] = heap[right];
					pos = right;
				}
				else break;
			}
			else		// 只有左孩子
			{
				if (key >= heap[left])
				{
					heap[pos] = heap[left];
					pos = left;
				}
				else break;
			}
		}
		else break;
	}
	heap[pos] = key;
}

// 向堆中插入一个元素, 同时保证堆中不超过 k 个元素
void insert(int *heap, int data)
{
	if (data <= heap[1]) return ;	// 元素没堆顶大
	heap[1] = data;
	node_down(1);		// 尝试下调堆顶
}

int main(void)
{
	char *path = "c:\\input.dat";
	FILE *in = NULL;
	int data, i;
	unsigned int t1, t2;

	printf("正在生成数据 %s\n", path);
	random_write(path);
	in = fopen(path, "rt");
	if (in == NULL) return 0;

	// 初始化堆
	init_heap();

	// 循环读入数据处理
	printf("正在处理数据\n", path);
	t1 = clock();
	while (fscanf(in, "%d", &data) != EOF)
	{
		insert(heap, data);
	}
	t2 = clock();
	printf("大最的前 %d 个元素:\n", k);
	for (i=1; i<=k; i++)
		printf("%d\n", heap[i]);
	printf("\n用时: %.3f 秒\n", (t2 - t1) / 1000.0);
	fclose(in);
	remove(path);

	return 0;
}