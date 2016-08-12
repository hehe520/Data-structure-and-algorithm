
// 尝试运行多个本程序, 观察并发控制
// 本程序每次只允许一个实例运行, 如果有多个同时运行将被阻塞
// 直到有一个运行中的退出, 于是下一个实例又可以运行

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

// 提示等待信息
DWORD WINAPI msg(void *id)
{
	int time = *(DWORD *)id;
	int i;

	system("cls");
	for (i=time/1000-1; i>=0; i--)
	{
		printf("\r等待进入中 %02d 秒", i);
		Sleep(1000);
	}
	return 0;
}

int main(void)
{
	HANDLE mutex;			// 信号量
	char name[] = "cpp1";	// 信号量名字
	int num = 1;			// 允许的最大运行数
	DWORD time = 30000;		// 最大等待时间, 30 秒
	DWORD flag;
	HANDLE tip;

	// 尝试打开一个 "cpp1" 的信号量
	mutex = OpenSemaphore(SEMAPHORE_ALL_ACCESS, true, name);
	if (mutex == NULL)		// 第一次运行, 打开失败
	{
		mutex = CreateSemaphore(NULL, num, 1000, name);
		if (mutex == NULL) return 0;		// 创建失败
	}
	// 提示信息的线程 tip
	tip = CreateThread(NULL, 0, msg, (void *)(&time), 0, NULL);
	Sleep(5);			// 给创建线程一点时间
	flag = WaitForSingleObject(mutex, time);		// 尝试进入
	if (flag == WAIT_OBJECT_0)
	{
		// 进入成功, 杀死信息提示线程并清屏
		TerminateThread(tip, 0);
		CloseHandle(tip);
		system("cls");
		printf("\n本进程正在占用临界区,,,\n");
		printf("\n按任意键退出...\n");
		getchar();
		ReleaseSemaphore(mutex, 1, NULL);
	}
	else if (flag == WAIT_TIMEOUT)
	{
		printf("\n\n等待超时, 请重试...\n");
		CloseHandle(tip);
		getchar();
	}
	CloseHandle(mutex);

	return 0;
}
/*
OpenSemaphore(SEMAPHORE_ALL_ACCESS, true, name)
参数一:
SEMAPHORE_ALL_ACCESS	对信号量完全控制, P, V 操作皆可
SEMAPHORE_MODIFY_STATE  允许使用ReleaseSemaphore(), 只允许 V 操作
参数二:
true		允许子进程继承该信号量
false		不允许继承
参数三:
name		要打开信号量的名字, 打开失败返回 NULL


DWORD WaitForSingleObject() 返回值
WAIT_OBJECT_0		得到信号量
WAIT_TIMEOUT		等待超时
WAIT_FAILED			出现错误
*/