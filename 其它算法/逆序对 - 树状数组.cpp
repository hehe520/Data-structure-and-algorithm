
// 利用树状数组求逆序对 
// 算法:
// 输入一组数据, 先进行离散化, 即缩小数字间的距离到 1,
// 例如 100, 20, 2000, 10, 转为 3, 2, 4, 1
// 即 4 代表 2000,   3 代表 1000,   2 代表 100
// 只需反映出数字大小关系即可, 和数字多少没关系
// 在对离散化后的, 3, 2, 4, 1 求逆序对
// 引入一个数组 c 记录每个数字后面比自己小的个数
// 如 c[1] = 2, 即 3 后面有 2, 1 比其小
// 所以上例的 c[] = {2, 1, 1, 0}
// 因为要统计自己后面的数哪个比自己小, 所以后面的数要先进入树状数组

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define LOWBIT(x) ((x) & (-x))

typedef struct 
{
	int data;
	int i;
}arr;

arr a[500];		// 离散化用的数组
int c[500];		// 树状数组
int n;			// 数组个数, 0 号不用

int cmp(const void *a, const void *b)	// 升序排序
{
	arr _a = *((arr *)a);
	arr _b = *((arr *)b);

	return _a.data > _b.data;
}

void add(int i, int d)		// 第 i 个元素增加 d
{
	while (i <= n)
	{
		c[i] += d;
		i += LOWBIT(i);
	}
}

int sum(int i)		// 前 n 项和
{
	int s = 0;
	
	while (i > 0)
	{
		s += c[i];
		i -= LOWBIT(i);
	}
	return s;
}

int main(void)
{
	int i;
	int ans = 0;

	printf("输入 n 及 n 个元素: ");
	scanf("%d", &n);
	for (i=1; i<=n; i++)
	{
		scanf("%d", &a[i].data);
		a[i].i = i;
	}
	qsort(a+1, n, sizeof(arr), cmp);
	// 排序后即离散化了, 下面用 a[i].i
	// 下面的 for 循环倒着写, 因为要求后面比自己小的数,
	// 所以后面的数应该先进入树状数组
	// 同理求前面比自己小的数, for 就应该正着写
	memset(c, 0, sizeof(c));		// 初始化树状数组
	for (i=n; i>0; i--)
	{
		ans += sum(a[i].i);		// 统计比自己小的总和
		add(a[i].i, 1);			// 把自己放入数组中
	}
	printf("逆序对: %d\n\n", ans);

	return 0;
}