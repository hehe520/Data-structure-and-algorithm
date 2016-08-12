
// 本程序将产生死锁
// t1 占用 A 资源, 申请 B 资源
// t2 占用 B 资源, 申请 A 资源

#include <stdio.h>
#include <windows.h>

HANDLE A, B;

DWORD WINAPI fun1(void *id)		// 占用 A 资源, 申请 B 资源
{
	WaitForSingleObject(A, INFINITE);
	Sleep(500);
	WaitForSingleObject(B, INFINITE);
	printf("该句话得不到输出\n");

	return 0;
}

DWORD WINAPI fun2(void *id)		// 占用 B 资源, 申请 A 资源
{
	WaitForSingleObject(B, INFINITE);
	Sleep(500);
	WaitForSingleObject(A, INFINITE);
	printf("该句话得不到输出\n");

	return 0;
}

int main(void)
{
	HANDLE t1, t2;

	A = CreateSemaphore(NULL, 1, 2, NULL);
	B = CreateSemaphore(NULL, 1, 2, NULL);
	printf("运行中...\n");
	t1 = CreateThread(NULL, 0, fun1, NULL, NULL, 0);
	t2 = CreateThread(NULL, 0, fun2, NULL, NULL, 0);
	
	Sleep(5000);		// 休眠 5 秒
	CloseHandle(t1);
	CloseHandle(t2);
	CloseHandle(A);
	CloseHandle(B);
	return 0;
}