#include <stdio.h>
#include <conio.h>
#include <windows.h>

int main(void)
{
	HANDLE mutex;

	// 创建一个名为 cpp1 互斥量
	// 如果已经存在 cpp1 的互斥量, 则创建失败
	mutex = CreateMutex(NULL, false, "cpp1");
	if (GetLastError() ==  ERROR_ALREADY_EXISTS)
	{
		printf("\n程序已经再运行了...\n");
		getch();
		CloseHandle(mutex);
		return 0;
	}
	printf("\n程序运行中...\n");
	getch();
	CloseHandle(mutex);
	return 0;
}
