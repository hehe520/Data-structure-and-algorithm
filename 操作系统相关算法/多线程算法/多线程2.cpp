#include <stdio.h>
#include <windows.h>

DWORD WINAPI func1(LPVOID para)
{
	int i;
	for (i=0; i<50; i++) printf("t1 = %d\n", i);
	return 0;
}

DWORD WINAPI func2(LPVOID para)
{
	int i;
	for (i=0; i<50; i++) printf(" t2 = %d\n", i);
	return 0;
}

int main(void)
{
	HANDLE t1, t2;

	// 两个线程同时运行, 抢时间片
	t1 = CreateThread(NULL, 0, func1, NULL, 0, NULL);
	t2 = CreateThread(NULL, 0, func2, NULL, 0, NULL);
	Sleep(100);			// 先不让 main 主进程结束
	CloseHandle(t1);
	CloseHandle(t2);
	printf("main 运行\n");

	return 0;
}
