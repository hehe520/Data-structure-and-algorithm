
// 控制 CPU 的使用率在 50%, 多核机器要指定一个核运行该程序

// GetTickCount()		// 系统开机到现在的毫秒数
// clock()				// 本进程到现在的毫秒数

#include <stdio.h>
#include <time.h>
#include <windows.h>

int main(void)
{
	DWORD st;
	while (1)
	{
		st = clock();
		// 运行 10 ms
		while (clock() - st <= 10);
		// 休息 10 ms
		Sleep(10);
	}
	return 0;
}