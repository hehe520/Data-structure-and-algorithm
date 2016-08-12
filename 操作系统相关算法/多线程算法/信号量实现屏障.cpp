#include <stdio.h>
#include <windows.h>

// 有 10 条线程在运行, 每个线程一个信号量
// 只有他们全部结束, main 线程才能结束

HANDLE mutex[10];
HANDLE m;			// 控制共享区访问

DWORD WINAPI func(void *id)
{
	int i, p;

	WaitForSingleObject(m, INFINITE);
	p = *((int *)id);
	printf(" %d 号线程正在运行...\n", p);
	for (i=0; i<100; i++);
	ReleaseSemaphore(mutex[p], 1, NULL);
	ReleaseSemaphore(m, 1, NULL);

	return 0;
}

int main(void)
{
	HANDLE t[10];
	int i;

	m = CreateSemaphore(NULL, 1, 11, NULL);
	for (i=0; i<10; i++)
		mutex[i] = CreateSemaphore(NULL, 0, 1, NULL);
	// 10 个信号量的初值全部设为 0, 表示还没运行完 (没信号)
	// 待10 个线程完了以后, 将 10 个信号量改为 1 (有信号)
	// 只有 10 信号都为 1 了, 才能结束 main 线程
	// WaitForMultipleObjects() 来等待多个信号

	for (i=0; i<10; i++) 
	{
		t[i] = CreateThread(NULL, 0, func, (void *)(&i), 0, NULL);
		Sleep(2);		// 给 CreateThread() 函数一点时间
	}
	// 等待 10 个信号, 收集齐了才返回
	WaitForMultipleObjects(10, mutex, true, INFINITE);
	for (i=0; i<10; i++) 
	{
		CloseHandle(t[i]);
		CloseHandle(mutex[i]);
	}
	CloseHandle(m);

	return 0;
}
/*		WaitForMultipleObjects() 说明
DWORD WaitForMultipleObjects(
DWORD nCount,				// 设置要等几个句柄；
CONST HANDLE *lpHandles,	// 句柄数组，把要等的句柄都放在这个数组里面
BOOL bWaitAll,				// 如果是 true 所有的句柄有信号时, 才运行
							// 如果是 false 任何一个句柄有信号, 就运行
DWORD dwMilliseconds		// 设置等待时间, 单位 ms, 无限等待 INFINITE 
)
*/
