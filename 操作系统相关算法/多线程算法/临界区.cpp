#include <stdio.h>
#include <windows.h>

// 临界区只能一个线程访问
// 临界区只能用来同步本进程内的线程，不可用来同步多个进程中的线程
// 原语操作:
// InitializeCriticalSection();	// 创建临界区
// DeleteCriticalSection();		// 销毁临界区
// EnterCriticalSection();		// 进入临界区
// LeaveCriticalSection();		// 离开临界区
// CRITICAL_SECTION mutex		// 创建一个临界区对象

int share[10];				// 临界区资源
CRITICAL_SECTION mutex;		// 临界区对象控制访问

DWORD WINAPI func1(void *id)	// 向 share 中写 0
{
	int i;

	EnterCriticalSection(&mutex);		// 尝试进入临界区
	printf("\nfunc1 在写入 0\n");
	for (i=0; i<10; i++) share[i] = 0;
	for (i=0; i<10; i++) printf("%d, ", share[i]);
	printf("\n");
	LeaveCriticalSection(&mutex);		// 离开

	return 0;
}

DWORD WINAPI func2(void *id)	// 向 share 中写 1
{
	int i;

	EnterCriticalSection(&mutex);
	printf("\nfunc2 在写入 1\n");
	for (i=0; i<10; i++) share[i] = 1;
	for (i=0; i<10; i++) printf("%d, ", share[i]);
	printf("\n");
	LeaveCriticalSection(&mutex);

	return 0;
}

int main(void)
{
	HANDLE t1, t2;

	InitializeCriticalSection(&mutex);		// 初始化临界对象
	t1 = CreateThread(NULL, 0, func1, NULL, 0, NULL);
	t2 = CreateThread(NULL, 0, func2, NULL, 0, NULL);

	Sleep(100);		// 估计线程运行时间
	// 也可用 WaitForMultipleObjects();  等待多个线程退出再退出
	
	CloseHandle(t1);
	CloseHandle(t2);
	DeleteCriticalSection(&mutex);		// 删除临界对象

	return 0;
}
