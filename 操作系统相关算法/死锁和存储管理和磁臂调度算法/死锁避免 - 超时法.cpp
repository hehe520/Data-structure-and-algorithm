
// t1 申请 A, A 被设成占用
// 超过 4 秒, 未得到该资源, 则放弃, 或者继续等待下一个 4 秒

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

HANDLE A, quit;
DWORD times = 4000;
// DWORD 是 unsigned long

DWORD WINAPI msg(void *id)		// 消息提示线程
{
	int n = *(int *)id;
	int i;
	
	for (i=n/1000; i>=0; i--)
	{
		printf("\r等待进入中 %d 秒... ", i);
		Sleep(1000);
	}
	return 0;
}

DWORD WINAPI fun1(void *id)		// t1 线程
{
	DWORD flag;
	HANDLE m;
	
	m = CreateThread(NULL, 0, msg, (void *)&times, 0, NULL);
	Sleep(5);
	flag = WaitForSingleObject(A, times);
	TerminateThread(m, 0);		// 杀死消息线程
	if (flag == WAIT_OBJECT_0)		// 成功拿到信号
	{
		system("cls");
		printf("\nt1 拿到信号\n");
	}
	else if (flag == WAIT_TIMEOUT)
	{
		printf("\n\nt1 等待超时\n");
	}
	CloseHandle(m);			// 释放消息线程句柄
	ReleaseSemaphore(quit, 1, NULL);	// 发退出信号
	return 0;
}

int main(void)
{
	HANDLE t1;
	
	// A 的信号量初始为 0, 可以设成 1 看效果
	A = CreateSemaphore(NULL, 0, 2, NULL);
	quit = CreateSemaphore(NULL, 0, 2, NULL);
	t1 = CreateThread(NULL, 0, fun1, NULL, 0, NULL);

	WaitForSingleObject(quit, INFINITE);		// 等待退出信号
	CloseHandle(t1);
	CloseHandle(A);

	return 0;
}