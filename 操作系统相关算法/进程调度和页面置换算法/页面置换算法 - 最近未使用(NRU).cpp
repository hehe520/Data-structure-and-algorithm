
// 虚拟储存技术中的页面置换算法 - 最近未使用 (NRU)
// 该算法基于页式的储存管理, 附带实验虚拟地址转换成物理地址

// 假设一个页面 1 KB, 内存大小 5 KB, 即物理空间只能存放 5 个页面
// 所以页框号只有 0 - 4 五个, 物理地址空间: 0000 ~ 5119
// 假设某程序有 10 个页面, 显然不能一次全部装进内存
// 该程序虚拟页面 0 - 9, 虚拟地址空间 00000 ~ 10239

// 程序在自己的地址空间随机访问, 模拟缺页的情况
// 可以想象程序中有很多跳转指令, 不一定顺序执行
// 程序流程, 随机产生一个虚拟地址, 经过页表转成物理地址
// 如果在页表中发现内存中没有这一页, 用置换算法换入该页
// 如果虚拟地址不在自己的地址空间, 可以提示一个 "0XFFFFF 该内存不能为 read"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

#define SIZE 10240		// 虚拟地址空间大小, 10 KB
#define PSIZE  1024		// 页大小 1 KB

// 页表, 10 个页面需要 10 个页表项
// 页表本身也在内存中, 在进程创建时, 由操作系统为该进程创建
struct
{
	unsigned visit  : 1;	// 访问位
	unsigned modify : 1;	// 修改位
	unsigned exist  : 1;	// 在不在位
	unsigned num    : 5;	// 页框号, 5 个位表示
}page[10];			// 10 个页面
int kuang = 5;		// 5  个页框

HANDLE mutex;		// 控制页表访问
int nopage = 0;		// 缺页次数

// 淘汰最近未使用页面, 返回可用的页框号
int NRU()
{
	int i;
	int value;
	int pos, min = 0XFFFFFF;
	
	// 淘汰顺序: 0 - 3
	// 0. 无访问, 无修改, 1. 无访问, 有修改
	// 2. 有访问, 无修改, 3. 有访问, 有修改
	// value = 访问位 * 2 + 修改位,  用权值比较
	// value 最小的淘汰
	for (i=0; i<10; i++)
	{
		if (page[i].exist)
		{
			value = page[i].visit * 2 + page[i].modify;
			if (value < min)
			{
				min = value;
				pos = i;
			}
		}
	}
	// 淘汰 pos 号页面, 如果被修改了, 还要写回磁盘
	printf("淘汰页面 %d, ", pos);
	page[pos].exist = 0;
	return page[pos].num;
}

// 把虚拟地址转换成物理地址, 若发生缺页, 还要调入该页
int visit(int vir_add)
{
	int vir_num;	// 页面号
	int phy_add;	// 物理地址

	if (vir_add < 0 || vir_add > SIZE-1) return -1;	// 地址越界
	// 在页表中查询物理地址
	vir_num = vir_add / PSIZE;	   // 页面号, 页内偏移是 vir_add % SIZE
	printf("页面 %d, ", vir_num);
	if (page[vir_num].exist == 0)
	{
		printf("未命中, ");
		// 不在内存, 引发缺页中断, 去页框中找一个可用的空间
		// 若找不到, 则置换掉最近未使用页面
		if (kuang) {page[vir_num].num = kuang--;  printf("            ");}
		else   page[vir_num].num = NRU();	// 得到一个空闲页框
		page[vir_num].exist = 1;
		nopage++;
	}
	else printf("命中  ,             ");
	printf("页框 %d, ", page[vir_num].num);
	page[vir_num].visit = 1;
	phy_add = page[vir_num].num;	// 取得页框号
	phy_add *= PSIZE;				// 乘上页面大小
	phy_add += vir_add % PSIZE;		// 加上页内偏移
	return phy_add;
}

// 每隔一个时钟滴答, 清空所有访问位
DWORD WINAPI clear(void *id)
{
	int i;
	while (1)
	{
		Sleep(5);		// 每隔 5 ms, 清空一次访问位
		WaitForSingleObject(mutex, INFINITE);
		printf("访问位定期清零被唤醒...\n");
		for (i=0; i<10; i++) page[i].visit = 0;
		ReleaseSemaphore(mutex, 1, NULL);
	}
	return 0;
}

int main(void)
{
	int times = 20;		// 访问次数
	int vir_add;		// 物理地址
	int phy_add;		// 虚拟地址
	int i;
	HANDLE t;

	mutex = CreateSemaphore(NULL, 1, 2, NULL);
	srand(time(NULL));
	for (i=0; i<10; i++)		// 初始化页表
	{
		page[i].visit = 0;
		page[i].modify = 0;
		page[i].exist = 0;
		page[i].num = 0;
	}
	t = CreateThread(NULL, 0, clear, NULL, 0, NULL);
	for (i=0; i<times; i++)
	{
		vir_add = rand() % SIZE;	// 随机访问一个地址
		WaitForSingleObject(mutex, INFINITE);
		printf("访问 %05d, ", vir_add);
		phy_add = visit(vir_add);
		printf("物理地址: %04d\n", phy_add);
		ReleaseSemaphore(mutex, 1, NULL);
	}
	SuspendThread(t);		// 赶紧挂起线程
	CloseHandle(t);
	CloseHandle(mutex);
	printf("\n访问次数 %d, 缺页次数 %d\n", times, nopage);
	return 0;
}