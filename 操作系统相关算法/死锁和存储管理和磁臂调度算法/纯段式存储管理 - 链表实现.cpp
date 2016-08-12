
// 纯段式存储管理的实现 - 使用链表的存储管理
// 拿 512 kB 空间用来实验, 512 kB = 524288 B
// 释放空间时要查看左右两个结点, 所以采用双向链表结构

// 主要函数:
// find_free_memory()		// 查找空闲内存
// free_memory()			// 释放内存
// collect_memory()			// 内存紧缩
// find_free_memory()		// 最佳适配
// find_free_memory_bad()	// 最差适配

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define FREE  0
#define USING 1
int total_size = 524288;	// 512 KB

// 双向链表结点
typedef struct node
{
	int add;			// 空间起始地址
	int size;			// 空间大小
	char flag;			// 空闲/占用位
	char name[21];		// 名字
	node *next;
	node *pre;
}node;
node *head;			// 表头
node *nullnode;		// 哑结点, 减少判断, 简化程序

// 创建新结点
node *new_node(int add, char flag, char *name, int size)
{
	node *n;

	n = (node *)malloc(sizeof(node));
	n->add = add;
	n->flag = flag;
	n->size = size;
	strcpy(n->name, name);
	n->pre = n->next = nullnode;
	return n;
}

// 内存紧缩
// 紧缩后, 同时要修改各个块的起始地址
// 遍历所有结点, 如果是占用, 将起始地址改为 add
// 如果是空闲, 记下空闲大小 size, 然后释放
void collect_memory()
{
	node *cur = head;
	node *del;
	int add = 0;		// add 初始为 0, 是一个增量
	int size = 0;
	int f = 1;
	node *pre;

	while (cur != nullnode)
	{
		if (cur->flag == USING)		// 占用的结点
		{
			cur->add = add;
			add += cur->size;
			if (f)		// 第一次要更改头结点
			{
				f = 0;
				head = cur;
				head->pre = nullnode;
				pre = head;
			}
			else	// pre 相当于织毛衣的针, 连接下一个结点
			{
				pre->next = cur;
				cur->pre = pre;
				pre = cur;
			}
			del = NULL;
		}
		else	// 空闲的结点, 记录大小并释放
		{
			size += cur->size;
			del = cur;
		}
		cur = cur->next;
		if (del) free(del);
	}
	pre->next = new_node(add, FREE, "null", size);
	pre->next->pre = pre;
	pre->next->next = nullnode;
}

// 寻找可用内存, 采用首次适配法
node *find_free_memory(int size, char *name)
{
	node *cur = head;
	node *n;

	while (cur != nullnode)
	{
		if (cur->flag == FREE && size <= cur->size)
		{	// 找到, 分裂 [cur] -> [n]
			n = new_node(cur->size-size, USING, name, size);
			cur->size -= size;
			cur->next->pre = n;			// 不用判断 cur-next 是否空
			n->next = cur->next;		// 有哑结点存在
			cur->next = n;
			n->pre = cur;
			return n;
		}
		cur = cur->next;
	}
	return NULL;
}

// 寻找可用内存, 采用最差适配法
node *find_free_memory_bad(int size, char *name)
{
	node *cur = head;
	int max_size = -1;
	node *pos = NULL;

	while (cur != nullnode)		// 找打一个最大空闲结点
	{
		if (cur->flag == FREE 
			&& cur->size > max_size 
			&& size <= cur->size)
		{
			max_size = cur->size;
			pos = cur;
		}
		cur = cur->next;
	}
	if (pos)		// 分裂 [pos] -> [cur]
	{
		cur = new_node(pos->size-size, USING, name, size);
		pos->size -= size;
		pos->next->pre = cur;
		cur->next = pos->next;
		pos->next = cur;
		cur->pre = pos;
		return cur;
	}
	return NULL;
}

// 合并右边的结点
// 合并前 [cur] -> [del] -> [x]
// 合并后 [cur] -> [x]
// 并且 cur 置为 空闲
void right_combine(node *cur)
{
	node *del = cur->next;

	cur->size += del->size;
	cur->flag = FREE;
	del->next->pre = cur;
	cur->next = del->next;
	free(del);
}

// 释放一块内存区域, 释放后, 要与左右空闲的结点合并
// 四种情况, 左右都闲, 仅左闲, 仅右闲, 左右都不闲
int free_memory(char *str)
{
	node *cur = head;
	char left, right;

	while (cur != nullnode)
	{
		if (!strcmp(str, cur->name))
		{
			// 找到, 进行释放, 判断左右结点的状态, 是空闲, 还是占用
			// 如果是空闲, 要与其合并, 哑结点不会空闲
			left = cur->pre->flag;
			right = cur->next->flag;
			switch (left + right * 2)
			{
			case 0 :	// 合并左右, 往左合并
				// [a] -> [cur] -> [cur->next] -> [b]
				right_combine(cur);			// 合并 [cur] 与 [cur->next]
				// [a] -> [cur] -> [b]
				cur = cur->pre;				// cur 去到 a 处
				// [a] -> [cur->next] -> [b]
				right_combine(cur);			// 合并 [a] 与 [cur->next]
				// [a] -> [b]
				break;
			case 1 :	// 与右合并, [cur] -> [n] -> [x],  [cur] -> [x]
				right_combine(cur);
				break;
			case 2 :	// 与左合并, [a] -> [cur] -> [x],  [a] -> [x]
				cur = cur->pre;
				right_combine(cur);
				break;
			case 3 :	// 无需合并
				cur->flag = FREE;
				break;
			}
			return 1;
		}
		cur = cur->next;
	}
	return 0;
}

// 统计可用大小, 也可设一个变量保存, 在申请, 释放时修改
int free_size()
{
	int size = 0;
	node *cur = head;

	while (cur != nullnode)
	{
		if (cur->flag == FREE) size += cur->size;
		cur = cur->next;
	}
	return size;
}

char display();		// 显示
void a();		// 申请内存
void b();		// 释放内存
void c();		// 内存紧缩
void d();		// 查看链表

int main(void)
{
	// 初始化表头, 哑结点
	nullnode = new_node(0, USING, "null", 0);
	head = new_node(0, FREE, "system", total_size);
	head->next = head->pre = nullnode;
	while (1)
	{
		switch (display())
		{
		case '1' : a();   break;		// 申请内存
		case '2' : b();   break;		// 释放内存
		case '3' : c();   break;		// 内存紧缩
		case '4' : d();   break;		// 查看链表
		default : return 0;				// 退出
		}
	}
	node *del;		// 释放链表
	while (head != nullnode)
	{
		del = head;
		head = head->next;
		free(del);
	}
	free(nullnode);
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
	printf("\n 4. 查看内存分配表\n");
	printf("\n---请输入: ");
	fflush(stdin);
	scanf("%c", &c);

	return c;
}

void a()	// 申请内存
{
	int size;
	char buf[21];
	node *n;

	system("cls");
	printf("内存总大小: %d 字节, 可用大小: %d 字节\n", 
		total_size, free_size());
	printf("\n申请内存的大小(字节): ");
	scanf("%d", &size);
	getchar();
	printf("\n给该内存区域取个名字 (最多 20 个字符): ");
	scanf("%s", buf);
	n = find_free_memory(size, buf);
//	n = find_free_memory_bad(size, buf);
	if (n != NULL) printf("\n申请成功");
	else printf("\n申请失败, 可尝试内存紧缩");
	back();
}

void b()	// 释放内存
{
	char buf[21];
	node *cur = head;

	system("cls");
	printf("\n已申请: \n");
	while (cur != nullnode)
	{
		if (cur->flag == USING) printf("%s\n", cur->name);
		cur = cur->next;
	}
	printf("\n输入待释放的变量名: ");
	fflush(stdin);
	scanf("%s", buf);
	if (free_memory(buf)) printf("\n成功释放");
	else printf("\n未找到 %s", buf);
	back();
}

void c()	// 内存紧缩
{
	system("cls");
	printf("\n正在内存紧缩...\n");
	collect_memory();
	printf("\n紧缩完毕\n");
	back();
}

void d()		// 查看链表
{
	node *cur = head;

	system("cls");
	printf("\n起始地址\t分配大小\t状态\t\t名字\n\n");
	while (cur != nullnode)
	{
		if (cur->flag == USING)
			printf("%d\t\t%d\t\t%s\t\t%s\n", 
				cur->add, cur->size, "using", cur->name);
		else 
			printf("%d\t\t%d\t\t%s\t\t%s\n", 
				cur->add, cur->size, "free", " ");
		cur = cur->next;
	}
	back();
}
