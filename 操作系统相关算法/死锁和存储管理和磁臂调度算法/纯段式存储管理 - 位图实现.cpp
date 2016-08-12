
// 纯段式存储管理的实现 - 使用位图的存储管理
// 假设拿 320 KB 的内存空间用来实验, 320 KB = 327680 B
// 使用一个字节来管理一个单元是否被使用
// 设单元大小 512 B, 那么位图大小是 320 KB / 512 B = 640 个位

// 上面的数值可以根据实际自定
// 单元大小越小, 需要的位图越大, 费空间
// 单元大小越大, 需要的位图越小, 同时产生的内部碎片多, 浪费空间

// 以 main 函数为分界线, main 以下的是界面等无关紧要的代码
// 主要函数:
// find_free_memory()		// 查找空闲内存
// free_memory()			// 释放内存
// collect_memory()			// 内存紧缩

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define NUM  640		// 位数量 (number)
#define UNIT_SIZE 512	// 单元大小 (unit size)
#define USING 1			// 占用
#define FREE  0			// 空闲

char map[NUM];			// 位图
int free_bit = NUM;

struct re		// 记录起始地址, 内存名字, 释放内存时用到
{
	unsigned int addr;		// 起始地址
	unsigned int number;	// 分配单元数量
	char name[21];			// 变量名字
}recond[100];
int top = 0;
// 与链表存储管理区别, 链表管理是记录字节数量
// 位图管理是记录单元数量, 记录位段的起始地址


// 执行内存紧缩
// 全部向起始地址靠拢, 如:
// 紧缩前: 0 0 1 1 0 1 0 1
// 紧缩后: 1 1 1 1 0 0 0 0
// 紧缩时, 同时要调整各个变量的起始地址, 先按起始地址排序
// 然后从 0 地址开始, 把后面的往前搬
void collect_memory()
{
	void sort_addr();
	int i;
	int d = 0, s;	// d (destination),  s (start)
	int end;

	sort_addr();		// 起始地址排序
	for (i=0; i<top; i++)	// 一个一个往前移
	{
		s = recond[i].addr;			// 起始
		end = s + recond[i].number;	// 结束
		if (s != d)
		{
			recond[i].addr = d;		// 同时修改新的起始地址
			while (s < end)			// 移动
			{
				map[d++] = map[s];
				map[s++] = FREE;
			}
		}
		else d = end;		// 不需要移动
	}
}

// 查找可用的内存, 采用首次适配法
// 若找不到合适的空间, 返回 0, 否则返回 1
// 也可以启动内存紧缩后, 再寻找内存, 但实际中不这么做, 内存紧缩很耗时
int find_free_memory()		// 参数信息在 recond[top]
{
	int i, j;
	int size;
	
	for (i=0; i<NUM; i++)
	{
		if (map[i] == FREE)		// 首次适配
		{
			// 看看连续的空间是否够
			size = recond[top].number + i;
			if (size > NUM) return 0;
			for (j=i; j<size; j++)
			{
				if (map[j] == USING) break;
			}	// 有一个被占用了, 就 break, 从 j 出重新找
			if (j == size)
			{	// j == size 说明找到了
				recond[top].addr = i;
				for (j=i; j<size; j++) map[j] = USING;
				free_bit -= recond[top].number;
				return 1;
			}
			else i = j + 1;
		}
	}
	return 0;
}

// 释放名为 str 的空间, 从起始地址开始全部写 FREE
int free_memory(char *str)
{
	int i, j;
	int size;

	for (i=0; i<top; i++)
	{
		if (!strcmp(recond[i].name, str))	// 找到
		{
			size = recond[i].number;
			j = recond[i].addr;
			while (size--) map[j++] = FREE;
			free_bit += recond[i].number;
			for (j=i; j<top; j++) recond[j] = recond[j + 1];
			top--;
			return 1;
		}
	}
	return 0;
}

char display();		// 显示
void a();		// 申请内存
void b();		// 释放内存
void c();		// 内存紧缩
void d();		// 查看位图

int main(void)
{
	memset(map, FREE, sizeof(map));  // 初始化位图为 FREE
	while (1)
	{
		switch (display())
		{
		case '1' : a();   break;		// 申请内存
		case '2' : b();   break;		// 释放内存
		case '3' : c();   break;		// 内存紧缩
		case '4' : d();   break;		// 查看位图
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
	printf("\n 1. 申请内存\n");
	printf("\n 2. 释放内存\n");
	printf("\n 3. 内存紧缩\n");
	printf("\n 4. 查看位图\n");
	printf("\n---请输入: ");
	fflush(stdin);
	scanf("%c", &c);

	return c;
}

void a()	// 申请内存
{
	unsigned int size;

	system("cls");
	printf("内存总大小: %d 字节, 可用大小: %d 字节\n", 
		NUM * UNIT_SIZE, free_bit * UNIT_SIZE);
	printf("\n申请内存的大小(字节): ");
	scanf("%d", &size);
	getchar();
	// 不检查变量名是否重名了, 查找不是本实验重点
	// 可以用顺序查找, 哈希查找, 字典树等等查找树表, 判断集合中重复元素
	printf("\n给该内存区域取个名字 (最多 20 个字符): ");
	scanf("%s", recond[top].name);
	recond[top].number = (size / UNIT_SIZE) + !!(size % UNIT_SIZE);
	if (find_free_memory()) printf("\n申请成功", top++);
	else printf("\n申请失败, 可尝试内存紧缩");
	back();
}

void b()	// 释放内存
{
	char buf[21];
	int i;

	system("cls");
	if (top)
	{
		printf("\n已申请: \n");
		for (i=0; i<top; i++)
			printf("%s\n", recond[i].name);
	}
	printf("\n输入待释放的变量名: ");
	fflush(stdin);
	scanf("%s", buf);
	if (free_memory(buf)) printf("\n成功释放 %s", buf);
	else printf("\n未找到 %s", buf);
	back();
}

void c()	// 内存紧缩
{
	system("cls");
	printf("\n正在内存紧缩...\n");
	collect_memory();
	printf("\n紧缩完毕, 可查看位图结果\n");
	back();
}

void d()		// 查看位图
{
	int i;

	system("cls");
	printf("0 表示占用, 1 表示空闲\n");
	for (i=0; i<NUM; i++)
	{
		if (i % 32 == 0) printf("\n");
		printf("%2d", map[i]);
	}
	back();
}

void sort_addr()	// 排序
{
	int i, j;
	char flag;
	struct re tmp;

	for (i=0; i<top; i++)
	{
		flag = 1;
		for (j=0; j<top-1; j++)
		{
			if (recond[j].addr > recond[j+1].addr)
			{
				tmp = recond[j];
				recond[j] = recond[j+1];
				recond[j+1] = tmp;
				flag = 0;
			}
		}
		if (flag) break;
	}
}
