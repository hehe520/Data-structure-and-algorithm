
// 磁盘空闲块管理, 位图法
// 文件储存空间不连续, 由文件分配表管理每个文件的块号
// 由于用了分块, 所以不会产生外部碎片, 不再需要内存紧缩
// 磁盘块大小 1 KB, 假设有 256 KB 磁盘空间
// 则需要 256 个位图来表示

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#define NUM  256		// 位数量 (number)
#define UNIT_SIZE 1024	// 单元大小 (unit size)
#define USING 1			// 占用
#define FREE  0			// 空闲
#define NO -1

char map[NUM];			// 位图
int free_bit = NUM;		// 可用快数

struct			// 目录
{
	char name[100];		// 文件名
	int start;			// 文件分配表的起始地址
}recond[100];
int top = 0;

int next[NUM + 10];		// 文件分配表, 其实就是静态链表
// 纯段式中的位图管理, 要求空间是连续的
// 而文件在磁盘上的储存可以不连续, 所以需要文件分配表来管理
// 文件分配表在内存中


// 分配一个大小为 size 字节的文件
// 返回文件分配表的首地址
int find_free_memory(int size)
{
	int count = size / UNIT_SIZE + !!(size % UNIT_SIZE);
	// 分配 count 个磁盘块
	int i;
	int start = 0;
	int pre;
	int f = 1;

	if (count > free_bit) return NO;	// 空间不够了
	else free_bit -= count;		// 减去分配的块数
	for (i=0; i<NUM; i++)
	{
		if (map[i] == FREE)
		{
			if (f)		// 如果是首次
			{
				f = 0;
				start = i;		// 起始块号
				pre = i;
			}
			else
			{
				next[pre] = i;		// 连接分配表
				pre = i;		// pre 去到下一个结点
			}
			map[i] = USING;
			if (count - 1 != 0) count--;
			else
			{
				next[pre] = NO;
				break;
			}
		}
	}
	return start;
}

// 删除名为 str 的文件
int free_memory(char *str)
{
	int i;
	int pre;
	int cur;

	for (i=0; i<top; i++)
	{
		if (!strcmp(str, recond[i].name))
		{
			cur = recond[i].start;
			while (cur != NO)
			{
				map[cur] = FREE;		// 位图释放
				free_bit++;
				pre = cur;
				cur = next[cur];
				next[pre] = NO;		// 擦除分配表内容
			}
			recond[i] = recond[top - 1];	// 替换删除
			top--;
			return 1;
		}
	}
	return 0;
}

int main(void)
{
	char display();		// 显示
	void a();		// 创建文件
	void b();		// 删除文件
	void c();		// 查看文件分配表
	void d();		// 查看位图

	memset(next, -1, sizeof(next));		// 初始化分配表
	memset(map, FREE, sizeof(map));		// 初始化位图为
	while (1)
	{
		switch (display())
		{
		case '1' : a();   break;
		case '2' : b();   break;
		case '3' : c();   break;
		case '4' : d();   break;
		default : return 0;				// 退出
		}
	}
	return 0;
}

// ------------------------------------------------------------
// ------------------下面是关于界面的无用代码------------------
// ------------------------------------------------------------
void back()
{
	printf("\n\n回车返回...\n");
	fflush(stdin);
	getchar();
}

char display()		// 显示
{
	char c;

	system("cls");
	printf("\n 1. 创建文件\n");
	printf("\n 2. 删除文件\n");
	printf("\n 3. 查看文件分配表\n");
	printf("\n 4. 查看位图\n");
	printf("\n---请输入: ");
	fflush(stdin);
	scanf("%c", &c);

	return c;
}

void a()	// 创建文件
{
	int size;

	system("cls");
	printf("磁盘总大小: %d 字节, 可用大小: %d 字节\n", 
		NUM * UNIT_SIZE, free_bit * UNIT_SIZE);
	printf("\n输入文件名: ");
	scanf("%s", recond[top].name);
	printf("\n文件大小(字节): ");
	scanf("%d", &size);
	recond[top].start = find_free_memory(size);
	if (recond[top].start != NO) printf("\n创建成功");
	else printf("\n创建失败, 磁盘空间不足");
	top++;
	back();
}

void b()	// 删除文件
{
	char buf[100];
	int i;

	system("cls");
	if (top)
	{
		printf("\n文件: \n");
		for (i=0; i<top; i++)
			printf("%s\n", recond[i].name);
	}
	printf("\n输入待删除的文件名: ");
	fflush(stdin);
	scanf("%s", buf);
	if (free_memory(buf)) printf("\n成功删除 %s", buf);
	else printf("\n未找到 %s", buf);
	back();
}

void c()	// 查看文件分配表
{
	int i;
	int cur;

	system("cls");
	printf("\n%-16s%-10s%-10s\n", "文件名", "起始块号", "占有块号");
	for (i=0; i<top; i++)
	{
		printf("%-16s ", recond[i].name, recond[i].start);
		cur = recond[i].start;
		while (cur != NO)
		{
			printf("%d, ", cur);
			cur = next[cur];
		}
		putchar('\n');
	}
	back();
}

void d()		// 查看位图
{
	int i;

	system("cls");
	printf("0 表示占用, 1 表示空闲\n");
	for (i=0; i<NUM; i++)
	{
		if (i % 16 == 0) printf("\n");
		printf("%3d", map[i]);
	}
	back();
}
