#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sqlist.h"

sqlist list;	// 顺序表
head linklist;	// 单链表

const char *listpath = "C:\\list.dat";
const char *nodepath = "C:\\node.dat";

void display()
{
	static int f = 1;	// 保证 f 在整个程序中不变

	system("color 0e");
	printf("\n---------线性表写入/读取文件\n\n");
	printf("--- 1. 顺序表写入文件\n");
	printf("--- 2. 从文件中读取顺序表\n\n");
	printf("--- 3. 链表写入文件\n");
	printf("--- 4. 从文件中读取链表\n\n");
	printf("--- 输入序号，执行操作：");
}

int getnum(char str[])
{
	int buf;
	char s[10] = "exit";

	if (str[0] >= '0' && str[0] <= '9')
	{
		sscanf(str, "%d", &buf);
		return buf;
	}
	else if (!strcmp(str, s)) return -2;
	else return -1;
}

void wait()
{
	char str[10];
	int op;
	
	while (1)
	{
		fflush(stdin);
		scanf("%9s", str);
		fflush(stdin);
		op = getnum(str);
		if (op >= 1 && op <= 4)
		{
			switch (op)
			{
			case 1 : exe_writefile();	break;
			case 2 : exe_readfile();	break;
			case 3 : exe_writefile2();	break;
			case 4 : exe_readfile2();	break;
			}
		}
		else if (op == -2) exit(0);
		else printf("XXX 输入错误，请重新输入：");
	}
}

void back()
{
	fflush(stdin);
	printf("\n\n--- 按下回车键，回主界面");
	getchar();
	system("cls");
	display();
}

void exe_writefile()	// 顺序表写文件
{
	int i = 1;
	unsigned char status = 0;
	char str[10];
	int num;

	system("cls");
	createlist(&list, SIZE);
	printf("\n--- 请输入数据，输入任意字母结束\n");
	while (1)
	{
		printf("--- 请输入第 %d 个：", i++);
		scanf("%9s", str);
		fflush(stdin);
		num = getnum(str);
		if (num == -1)
		{
			status = 0;
			break;
		}
		// 插到 list 的最后
		if (!insertlist(&list, list.lenth+1, &num)) 
		{
			status = 1;
			break;
		}
	}
	if (!status) printf("\n--- 结束输入\n");
	else printf("--- 插入失败，输入结束\n");
	
	// 开始写入文件...
	if (writefile(&list, listpath))
		printf("\n--- 文件保存成功!\n位置：%s", listpath);
	else printf("\n--- 写入文件失败\n");
	destorylist(&list);
	back();
}

void exe_readfile()		// 从文件读取顺序表
{
	system("cls");
	if (readfile(&list, listpath))
	{
		printf("\n--- 读取成功！\n\n");
		showlist(&list);
	}
	else printf("\n--- 读取失败, 文件不存在!\n");

	// 将list编辑后，又存回文件，待开发...

	back();
}

void exe_writefile2()	// 链表写文件
{
	int i = 1;
	unsigned char status = 0;
	char str[10];
	int num;

	system("cls");
	createnode(&linklist);
	printf("\n--- 请输入数据，输入任意字母结束\n");
	while (1)
	{
		printf("--- 请输入第 %d 个：", i++);
		scanf("%9s", str);
		fflush(stdin);
		num = getnum(str);
		if (num == -1)
		{
			status = 0;
			break;
		}
		// 插到 list 的最后
		if (!append(&linklist, &num)) 
		{
			status = 1;
			break;
		}
	}
	if (!status) printf("\n--- 结束输入\n");
	else printf("--- 插入失败，输入结束\n");

	// 开始写入文件，一个一个结点地写入
	if (writefile2(&linklist, nodepath))
		printf("\n--- 文件保存成功!\n位置：%s", nodepath);
	else printf("\n--- 写入文件失败\n");
	destorynode(&linklist);

	back();
}

void exe_readfile2()	// 从文件读取链表
{
	system("cls");

	createnode(&linklist);
	if (readfile2(&linklist, nodepath))
	{
		printf("\n--- 读取成功！\n\n");
		shownode(&linklist);
	}
	else printf("\n--- 读取失败, 文件不存在!\n");
	destorynode(&linklist);

	back();
}
