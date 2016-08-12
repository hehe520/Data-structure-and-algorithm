// 存放 main 里的函数
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sqlist.h"

void display()
{
	system("color 0e");
	printf("\n---------顺序表的存储及操作：\n\n");

	printf("--- 状态：%s\n\n", 
		list.elem==0?"顺序表未创建":"顺序表已创建");
	printf("--- 1. 创建线性表\n");
	printf("--- 2. 依次输入数据\n");
	printf("--- 3. 销毁线性表\n\n");

	printf("--- 4. 显示顺序表\n");
	printf("--- 5. 显示表长\n\n");

	printf("--- 6. 插入元素\n");
	printf("--- 7. 修改元素\n");
	printf("--- 8. 删除元素\n\n");

	printf("--- 9. 重置全部元素\n");
	printf("--- 10. 删除全部数据\n");
	printf("--- 11. 访问第N个元素\n\n");

	printf("--- 输入序号，执行操作\n");
	printf("--- 输入exit，退出程序：");
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
		memset(str, 0, 10*sizeof(char));
		scanf("%8s", str);
		fflush(stdin);
		op = getnum(str);
		if (op>=1 && op<=11)
		{
			switch (op)
			{
			case 1 : exe_createlist();	break;
			case 2 : exe_input();		break;
			case 3 : exe_destorylist();	break;
			case 4 : exe_showlist();	break;
			case 5 : exe_listlen();		break;
			case 6 : exe_insert();		break;
			case 7 : exe_update();		break;
			case 8 : exe_remove();		break;
			case 9 : exe_clearlist();	break;
			case 10: exe_delall();		break;
			case 11: exe_getelem();		break;
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

void exe_createlist()
{
	system("cls");
	if (createlist(&list)) printf("\n--- 创建成功！\n");
	else printf("\n--- Error:创建失败！\n\n--- 表已经创建了！\n");
	back();
}

void exe_input()
{
	int i = 0;
	char buf[10];
	elemtype data;

	system("cls");
	if (list.elem == NULL) 
		printf("\nXXX 输入失败! 表还没有创建!\n");
	else 
	{
		printf("\n--- 输入任意字母，表示结束输入\n\n");
		while (1)
		{
			i++;
			printf(">>> 请输入第 %d 个数据：", i);
			memset(buf, 0, 10*sizeof(char));
			scanf("%8s", buf);
			if (buf[0]>='0' && buf[0]<='9')
			{
				sscanf(buf, "%d", &data);
				if (insert(&list, i, &data) == ERROR) 
				{
					printf("\nXXX ERROR：数据写入失败!\n");
					break;
				}
			}
			else 
			{
				printf("\n--- 输入结束!\n");
				break;
			}
		}
	}
	back();
}

void exe_destorylist()
{
	system("cls");
	if (list.elem == NULL) 
		printf("\nXXX 销毁失败! 表还没有被创建!\n");
	else 
	{
		if (destorylist(&list)) 
			printf("\n--- 销毁成功!\n");
		else printf("\n-- 销毁失败!\n");
	}
	back();
}
void exe_insert()
{	
	int i;
	elemtype data;

	system("cls");
	if (list.elem == NULL) 
		printf("\nXXX 插入失败! 表还没有被创建!\n");
	else 
	{
		printf("\n--- 当前表中元素：");
		if (!showlist(&list)) printf("空表!\n");
		printf("\n\n--- 请输入要插入的位置：");
		scanf("%d", &i);
		printf("\n--- 请输入要插入的数据：");
		scanf("%d", &data);
		if (insert(&list, i, &data))
		{
			printf("\n--- 插入成功！\n\n--- 当前表：");
			showlist(&list);
		}
		else 
		{
			printf("\n--- 插入失败！位置不正确！\n\n--- 当前表:");
			showlist(&list);
		}
	}
	back();
}
void exe_update()
{
	int pos;
	elemtype data;

	system("cls");
	if (list.elem == NULL) 
		printf("\nXXX 修改失败! 表还没有被创建!\n");
	else 
	{
		printf("\n--- 当前表中元素：");
		if (!showlist(&list)) printf("空表!\n");
		printf("\n\n--- 请输入要修改的位置：");
		scanf("%d", &pos);
		printf("\n--- 请输入新的数据：");
		scanf("%d", &data);
		if (update(&list, pos, &data))
		{
			printf("\n--- 修改成功！\n\n--- 当前表：");
			showlist(&list);
		}
		else 
		{
			printf("\n--- 修改失败！位置不正确！\n\n--- 当前表:");
			showlist(&list);
		}
	}
	back();
}

void exe_remove()
{
	int pos;

	system("cls");
	if (list.elem == NULL) 
		printf("\nXXX 删除失败! 表还没有被创建!\n");
	else 
	{
		printf("\n--- 当前表中元素：");
		if (!showlist(&list)) printf("空表!\n");
		printf("\n\n--- 请输入要删除的位置：");
		scanf("%d", &pos);
		if (remove(&list, pos))
		{
			printf("\n--- 删除成功！\n\n--- 当前表：");
			showlist(&list);
		}
		else 
		{
			printf("\n--- 删除失败！位置不正确！\n\n--- 当前表:");
			showlist(&list);
		}
	}
	back();
}

void exe_showlist()
{
	system("cls");
	if (list.elem == NULL) 
		printf("\nXXX 显示失败! 表还没有被创建!\n");
	else 
	{
		printf("\n--- 当前表中元素：");
		if (!showlist(&list)) printf("空表!\n");
	}
	back();
}

void exe_clearlist()
{
	system("cls");
	if (list.elem == NULL) 
		printf("\nXXX 清空失败! 表还没有被创建!\n");
	else 
	{
		if (clearlist(&list))
		{
			printf("\n--- 清空成功！\n\n");
			printf("\n--- 当前表中元素：");
		}
		if (!showlist(&list)) printf("空表!\n");
	}
	back();
}
void exe_getelem()
{
	int pos;
	elemtype data;

	system("cls");
	if (list.elem == NULL) 
		printf("\nXXX 访问失败! 表还没有被创建!\n");
	else 
	{
		listlen(&list, &pos);
		printf("\n--- 当前表长：%d\n\n", pos);
		printf("\n--- 请输入要访问的位置：");
		scanf("%d", &pos);
		if (getelem(&list, pos, &data))
			printf("\n--- 该数据为：%d\n\n", data);
		else 
			printf("\n--- 访问失败！位置不正确！\n\n");
	}
	back();
}
void exe_listlen()
{
	int pos;

	system("cls");
	if (list.elem == NULL) 
		printf("\nXXX 访问失败! 表还没有被创建!\n");
	else 
	{
		listlen(&list, &pos);
		printf("\n--- 当前表长：%d\n\n", pos);
	}
	back();
}

void exe_delall()
{
	int pos;
	int i;

	system("cls");
	if (list.elem == NULL) 
		printf("\nXXX 删除失败! 表还没有被创建!\n");
	else 
	{
		listlen(&list, &pos);
		// 从后面往前米删，复杂度小
		for (i=pos; i>=1; i--)
		{
			remove(&list, i);
		}
		listlen(&list, &pos);
		printf("\n--- 删除完成！当前表长：%d\n", pos);
	}
	back();
}

