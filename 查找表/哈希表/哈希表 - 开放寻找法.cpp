
// 哈希表解决冲突 - 开放寻址法

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

typedef int keytype;	// 采用数值数据
typedef int status;
#define OK 1
#define ERROR 0
#define FOUND -1
#define NO -1		// 结束标志
#define R 11		// 取余因子

// 哈希表大小的素数列, NO 表示没得拓展了
// 哈希表要开到更大, 只需修改数组, 这里开到 97 就不再开了
int hashsize[] = {11, 19, 29, 37, 47, 59, 67, 83, 97, NO};

typedef struct 
{
	keytype *base;	// 基址
	int count;		// 元素个数
	int psize;		// 总大小的指针
}hashtable;


status create(hashtable *t)		// 建立哈希表
{
	int i;
	if (!t) return ERROR;
	
	t->count = t->psize = 0;
	t->base = (keytype *)malloc(
		hashsize[t->psize]* sizeof(keytype));
	if (t->base)
	{
		for (i=0; i<hashsize[t->psize]; i++)
			t->base[i] = NO;	// NO 没有元素
	}
	return OK;
}

void destory(hashtable *t)		// 销毁哈希表
{
	if (t)
	{
		t->count = t->psize = 0;
		if (t->base) free(t->base);
	}
}

// 计算 n 的哈希值, 这里用取余法
// 类似的还有数字分析法, 折中法, 首尾相加法......
// 根据数据特征, 构造一个"合适"的哈希函数即可
inline int hash(int n)
{
	return n % R;		// 对 R 取余
}
/* 再举一个 PASCAL 语言关键字的 hash 函数例子
// 头尾字符的 ASCLL 码加起来做 hash 地址
int hash(char *str)
{
	return str[0] + str[strlen(str) - 1];
}
*/

inline void nextadd1(int *n)   // 线性探测
{ (*n)++; }	

inline void nextadd2(int *n, int c)   // 平方探测
{
	static int f = -1;
	f = -f;   c /= 2;		// 例如 +1, -1 要探测两次, 所以 c/2
	(*n) = f * c * c;
}

inline void nextadd3(int *n, int c)   // 斐波那契探测
{
	static int fib[] = {1,2,3,5,8,13,21,34,55,89,97};
	static int f2 = -1;

	f2 = -f2;   c /= 2;
	(*n) = f2 * fib[c - 1];
}

// 在哈希表中查找一个元素, 返回可插入位置, 也可以返回冲突次数 c
status search(hashtable *t, keytype data, int *pos)
{
	int add, c = 1;
	char flag = 1;		// 地址溢出标志

	add = hash(data);		// 计算 data 的哈希值
	while (t->base[add] != NO && t->base[add] != data)
	{
		// 当有冲突, 且不是相同的元素时, 继续探测下一哈希地址
		c++;					// 冲突次数
		nextadd1(&add);			// 线性探测
//		nextadd2(&add, c);		// 平方探测
//		nextadd3(&add, c);		// 斐波那契探测
		// 以上三种探测法, 全部测试通过, 任选一种即可
		// 若采用别的探测, 下面的 nextadd1() 要改, 并且 ASL 函数不可用!
		if (add < 0)
		{
			c++;   nextadd1(&add);		// 地址是负数了
		}
		if (add >= hashsize[t->psize])		// add 溢出
		{
			flag = 0;   break;
		}
	}
	*pos = add;
	if (flag && t->base[add] == data) return OK;
	else return ERROR;
}

// 向哈希表中插入一个元素
status insert(hashtable *t, keytype data)
{
	int add, i;

	if (search(t, data, &add)) return FOUND;	// 存在了
	else 
	{	// 哈希表不够大了, 追加空间, 直到放得下
		while (add >= hashsize[t->psize])
		{
			if (hashsize[t->psize] != NO)
			{
				t->psize++;
				t->base = (keytype *)realloc(t->base, 
					hashsize[t->psize] * sizeof(keytype));
				for (i=hashsize[t->psize-1]; i<hashsize[t->psize]; i++)
					t->base[i] = NO;
			}
			else return ERROR;
		}
		t->base[add] = data;	// 插入
		t->count++;
	}
	return OK;
}

// 求平均查找长度
// 根据实际位置与理想位置的偏差值计算查找长度
// 该方法只限于线性探测法的求 ASL
float ASL(hashtable *t)
{
	int i, len = 0;
	for (i=0; i<hashsize[t->psize]; i++)
	{
		if (t->base[i] != NO)
			len += i - t->base[i] % R + 1;
		// 实际在 i, 理想在 n % R
	}
	return (float)len / t->count;
}

void show(hashtable *t)		// 输出哈希表
{
	int i;
	for (i=0; i<hashsize[t->psize]; i++)
		if (t->base[i] != NO)
			printf("%d, ", t->base[i]);
		else printf("-, ");
}

int main(void)
{
	hashtable table;
	keytype data;
	status s;

	create(&table);
	while (1)		// 插入测试
	{
		printf("请输入要查找的元素: ");
		scanf("%d", &data);
		s = insert(&table, data);		// 插入

		if (s == OK) printf("\n该元素不存在, 已插入!");
		else if (s == FOUND) printf("\n已找到!");
		else printf("\n插入失败!");

		printf("\n\n哈希表: ");   show(&table);
		printf("\n\nASL = %.3f\t填装因子 = %.3f", 
			ASL(&table), (float)table.count / hashsize[table.psize]);
		printf("\n\n按 c 退出, 按回车继续...\n");
		getchar();
		if (getch() == 'c') break;
		system("cls");
	}
	// 删除一个元素不再测试, 只需通过 search() 找到这个元素, 再赋值 NO 即可
	destory(&table);
	return 0;
}