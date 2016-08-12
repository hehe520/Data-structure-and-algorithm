
// 基于二叉堆的优先队列

// 二叉堆的基本操作 - 小顶堆
// create()			建立堆
// destory()		销毁堆
// search()			查找一个元素
// insert()			插入一个元素
// deletetop()		删除堆顶
// getttop()		获得堆顶
// remove()			删除任意元素
// adjust()			调整优先级
// check()			检查堆的正确性
// down_node()		上调一个结点
// up_node()		下调一个结点
// create_all()		任意数组建堆

#include <stdio.h>
#include <stdlib.h>

typedef int elemtype;
typedef int status;
#define OK 1 
#define ERROR 0
#define MIN -0x4FFFFFFF		// 无穷小
#define SIZE 50				// 堆的初始大小
#define INC 20				// 扩建增量

// 二叉堆的存储结构
typedef struct
{
	elemtype *base;		// 基址
	int lenth;			// 使用量
	int size;			// 总容量
}heap;


status create(heap *h)		// 建立堆
{
	h->base = (elemtype *)malloc((SIZE+1) * sizeof(elemtype));
	h->base[0] = MIN;	// 0 号单元放无穷小, 方便后面处理
	h->lenth = 0;
	h->size = SIZE;
	return OK;
}

void destory(heap *h)		// 销毁堆
{
	if (h->base) free(h->base);
	h->lenth = h->size = 0;
}

int search(heap *h, elemtype data)	// 顺序查找一个元素
{
	int i;
	for (i=1; i<=h->lenth; i++)
		if (h->base[i] == data) break;
	if (i == h->lenth + 1) return -1;
	else return i;
}


// 上调一个结点
inline void up_node(heap *h, int p)
{
	int data = h->base[p];	// add 待上调的元素

	// 开始调整. p / 2 的下界是双亲节点, 2p, 2p+1 是孩子
	// 小顶堆, 小的要上浮, 大顶堆正好相反
	// 先找个合适的位置, 再插入, 减少赋值操作
	while (data < h->base[p / 2])
	{
		h->base[p] = h->base[p / 2];   // 大的下沉
		p /= 2;
	}
	h->base[p] = data;	// 腾出了位置, 插入
}

// 下调一个结点, p 是待下调的结点
// 下调时, 要注意是否有孩子, 有几个孩子, 孩子是否要上浮
inline void down_node(heap *h, int p)
{
	int len = h->lenth;
	int lc, rc;			// 左右孩子
	elemtype last = h->base[p];	// 获得待调整的结点, 也是为了减少赋值

	while (1)
	{
		// 要随时注意, 是否还有孩子结点
		// 孩子哪个小, 小的结点与 last 比较, 还是小就上浮, 否则退出
		lc = 2 * p;   rc = lc + 1;
		if (lc > len) break;		// 没有孩子
		else	// 肯定有左孩子
		{
			if (rc > len)		// 没有右孩子
			{
				// 左孩子小就上浮, 不小了, 就可以退出了
				if (h->base[lc] < last)
				{
					h->base[p] = h->base[lc];
					p = lc;		// 向左孩子走一步
				}
				else break;
			}
			else	// 左右都有, 挑个小的
			{
				// 在左右孩子中, 选一个小的, 且小于 last 的上浮
				if (h->base[lc] < h->base[rc])
				{
					if (h->base[lc] < last)
					{
						h->base[p] = h->base[lc];
						p = lc;		// add 是双亲
					}
					else break;
				}
				else	// 右孩子小
				{
					if (h->base[rc] < last)
					{
						h->base[p] = h->base[rc];
						p = rc;
					}
					else break;
				}
			}
		}
	} // end while
	h->base[p] = last;
}

// 在堆中插入一个元素, 同时调整堆 O(log N)
status insert(heap *h, elemtype data)
{
	if (h->lenth + 1 >= h->size)	// 堆满了, 扩建
	{
		h->base = (elemtype *)realloc(h->base, 
			(h->size + INC) * sizeof(elemtype));
		h->size += INC;
	}
	h->lenth++;					// 加一个新空间
	h->base[h->lenth] = data;	// 插入 data, 此时可能破坏堆性质
	up_node(h, h->lenth);		// 调整 data 的位置, 保证堆性质
	return OK;
}

// 删除任意一个元素, 同时调整堆, O(log N)
// 将最后一个元素, 替换到待删除的结点处, 再进行下沉操作
// 不是真正的替换, 因为赋值要花时间, 暂时逻辑上替换即可
status remove(heap *h, int add)
{
	if (add < 1 || add > h->lenth) return ERROR;

	h->base[add] = h->base[h->lenth];	// 把最后一个放到堆顶
	down_node(h, add);		// 再调整堆顶
	h->lenth--;		// 记得删掉最后一个
	return OK;
}

// 获得堆顶元素
status gettop(heap *h, elemtype *data)
{
	if (h->lenth == 0) return ERROR;	// 空了

	*data = h->base[1];
	return OK;
}

// 删除堆顶元素
status deletetop(heap *h)
{
	if (remove(h, 1)) return OK;
	else return ERROR;
}

// 调整优先级 
// 小顶堆, 正数降低优先级, 负数提高优先级, 大顶堆相反
status adjust(heap *h, elemtype data, int f)
{
	int add = search(h, data);

	if (add == -1) return ERROR;

	// 加上增量 f, 此时 add 结点可能有问题, 进行调整 
	h->base[add] += f; 
	if (f == 0) return OK;
	else if (f > 0) down_node(h, add);	// 上调操作
	else up_node(h, add);				// 下调操作

	return OK;
}

// 任意数组建堆
// 逆向遍历堆的数组, 全部进行上调
void create_all(heap *h)
{
	int i;
	for (i=h->lenth; i>0; i--) up_node(h, i);
}

// 检测是否符合小顶堆性质, 符合返回 ok
status check(heap *h, int i)
{
	char a = 1, b = 1;
	
	if (2 * i <= h->lenth)		// 如果有左孩子
	{
		if (h->base[i] > h->base[2*i]) return ERROR;
		a = check(h, 2 * i);	// 递归检测左孩子
	}
	if (2 * i + 1 <= h->lenth)	// 如果有右孩子
	{
		if (h->base[i] > h->base[2*i + 1]) return ERROR;
		b = check(h, 2 * i + 1);
	}
	return a && b;
}

int main(void)
{
	heap h;
	int i, n;
	int add, f;
	elemtype data;

	// 插入 n 个元素后, 不进行其它操作, 全部出队, 就是优先队列了
	create(&h);
	printf("请插入 n 个元素: ");
	scanf("%d", &n);
	for (i=0; i<n; i++)
	{
		scanf("%d", &data);		// 边建边调
//		data = rand() % 100;	// (测试用) 不想手动输入, 就自动吧
		insert(&h, data);		// 插入测试
	}
	printf("\n\n弹出 n 个元素: ");
	scanf("%d", &n);
	for (i=0; i<n; i++)
	{
		if (gettop(&h, &data))
		{
			printf("%d, ", data);
			deletetop(&h);		// 同时删除堆顶
		}
		else break;
	}
	printf("\n\n删除一个元素: ");
	scanf("%d", &data);
	add = search(&h, data);
	if (add != -1 && remove(&h, add))	// 删除测试
		printf("删除成功!\n");
	else printf("没有该元素!\n");

	printf("\n调整 x 的优先级, 调幅 f 如(x f): ");
	scanf("%d %d", &data, &f);
	if (adjust(&h, data, f)) printf("调整成功!\n");
	else printf("没有该元素!\n");
	
	printf("\n弹出表中剩余的元素: ");
	while (h.lenth != 0)
	{
		gettop(&h, &data);
		printf("%d, ", data);
		deletetop(&h);
	}
	destory(&h);	// 销毁原来的
	create(&h);
	printf("\n\n输入一个数组, 统一建堆: ");
	scanf("%d", &n);
	for (i=1; i<=n; i++)
	{
		scanf("%d", h.base + i);
//		h.base[i] = rand() % 100;   // (测试用)
	}
	h.lenth = i - 1;
	create_all(&h);		// 调整任意数组到满足堆性质
	printf("\n弹出表中剩余的元素: ");
	while (h.lenth != 0)
	{
		gettop(&h, &data);
		printf("%d, ", data);
		deletetop(&h);
	}
	printf("\n\n");
	destory(&h);
	return 0;
}
