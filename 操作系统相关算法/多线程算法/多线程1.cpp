#include <stdio.h>
#include <windows.h>

DWORD WINAPI func(LPVOID para)
{
	printf("func 运行 %d\n", para);
	return 0;
}

int main(void)
{
	HANDLE thread1;

	thread1 = CreateThread(NULL, 0, func, NULL, 0, NULL);
	Sleep(100);
	// main 主进程睡眠 100 ms, 让 thread1 有机会得到时间片运行
	// 如果不加 Sleep 会得到奇怪的结果, 或者整个进程都结束了
	CloseHandle(thread1);		// 关闭线程
	printf("main 运行\n");

	return 0;
}