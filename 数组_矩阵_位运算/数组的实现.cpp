#include <stdio.h>
#include <malloc.h>
#include <stdarg.h>

typedef int status;
typedef int elemtype;

#define OK 1
#define ERROR 0
#define OVERFLOW -1	// 地址溢出
#define MAX_DIM 10	// 最大维度 10 维

// 数组的顺序储存结构
typedef struct
{
	elemtype *base;		// 数组的基址
	int dim;			// 记录维数
	int *bound;			// 记录维度，如 int a[10][10];
	int *dp;			// 函数映像常量，类似动态规划的记录数组
}array;

status create(array *a, int dim, ...)
{
	// 检查维度
	// 根据维度，创建每维的维界
	// 根据每维的维界，计算空间总大小并创建
	// 计算函数映像常量，为了避免每次重复的计算
	
	int i;
	int size;
	va_list aq;

	// 开始检查 dim...
	if (dim <= 0 && dim > MAX_DIM) return ERROR;
	a->dim = dim;

	// 开始记录每维维界...
	a->bound = (int *)malloc(dim * sizeof(int));
	if (!a->bound) return ERROR;
	size = 1;
	va_start(aq, dim);		// 从 aq 栈中提取参数
	for (i=0; i<dim; i++)	// 记录每维维界
	{
		a->bound[i] = va_arg(aq, int);
		size *= a->bound[i];	// 顺便计算总大小
	}
	va_end(aq);

	// 开始创建总空间...
	a->base = (elemtype *)malloc(size * sizeof(int));
	if (!a->base) return ERROR;

	// 开始计算函数映像常量...
	a->dp = (int *)malloc(dim * sizeof(int));
	if (!a->dp) return ERROR;
	a->dp[dim-1] = 1;
	for (i=dim-2; i>=0; i--)
	{
		a->dp[i] = a->dp[i+1] * a->bound[i+1];
		// 该句话很像 DP 的思想，用空间换时间
	}
	return OK;
}

status destory(array *a)
{
	if (a->base) free(a->base);
	a->base = NULL;
	if (a->bound) free(a->bound);
	a->bound = NULL;
	if (a->dp) free(a->dp);
	a->dp = NULL;
	
	return OK;
}

// 寻找相对地址, 找不到就返回 OVERFLOW
// 把整个参数栈 aq 拿过来看看，
int find(array *a, va_list *aq)
{
	// 看看 a 是多少维的，我才可以提取多少参数
	// 根据多少参数，我才可以计算地址
	// 函数映像 dp 在此处可以避免重复计算，
	
	int address;	// 相对地址
	int border;		// 当前维界
	int i;

	address = 0;
	for (i=0; i<a->dim; i++)
	{
		border = va_arg(*aq, int);
		// 检查维界
		if (border<0 || border>=a->bound[i]) return OVERFLOW;
		address += border * a->dp[i];
		// border 相当于权值
	}
	return address;
}

// 修改值，就是 a[1][2][3] = 100;
status update(array *a, elemtype *data, ...)
{
	// 找到相对地址
	// 变址访问，然后可修改
	va_list aq;
	int address;

	va_start(aq, data);
	address = find(a, &aq);
	va_end(aq);
	if (address < 0 || address >= a->bound[a->dim]*a->dp[a->dim]) return ERROR;
	*(a->base + address) = *data;

	return OK;
}

// 取值，就是 b = a[1][2][3]
status getvalue(array *a, elemtype *back, ...)
{
	va_list aq;
	int addr;

	va_start(aq, back);
	addr = find(a, &aq);
	va_end(aq);
	if (addr < 0) return ERROR;
	*back = *(a->base + addr);

	return OK;
}

int main(void)
{
	array a;
	elemtype data;
	int i, j;
	elemtype num = 0;

	create(&a, 2, 5, 10);
	data = 520;
	if (update(&a, &data, 2, 3)) printf("赋值成功!\n");
	else printf("赋值失败!\n");
	if (update(&a, &data, 2, 17)) printf("赋值成功!\n");
	else printf("赋值失败!\n");

	if (getvalue(&a, &data, 2, 3)) printf("取值成功：%d\n", data);
	else printf("取值失败!\n");
	if (getvalue(&a, &data, 7, 2)) printf("取值成功：%d\n", data);
	else printf("取值失败!\n");

	// 赋值，取值测试通过
	for (i=0; i<5; i++)
	{
		for (j=0; j<10; j++)
		{
			update(&a, &num, i, j);
			num++;
		}
	}
	for (i=0; i<5; i++)
	{
		for (j=0; j<10; j++)
		{
			getvalue(&a, &num, i, j);
			printf("%3d", num);
		}
		printf("\n");
	}
	destory(&a);

	return 0;
}