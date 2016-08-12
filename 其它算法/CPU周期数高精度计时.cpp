
#include <stdio.h>
#include <windows.h> 

int main(void)
{
	// typedef __int64 LARGE_INTEGER
    LARGE_INTEGER start, end;             
    LARGE_INTEGER freq;   

	// 固定线程在 CPU_0 上，防止迁移
	SetThreadAffinityMask(GetCurrentThread(), 0x1);

	// 每秒钟 CPU 的周期数, 即每秒钟执行多少条指令
	// 一纳秒大约执行 3 条指令
	QueryPerformanceFrequency(&freq);
 	printf("CPU周期 = %I64d\n", freq.QuadPart);

	// CPU 运行到现在的周期总数
    QueryPerformanceCounter(&start);
    Sleep(1000);
    QueryPerformanceCounter(&end);

	printf("运行时间: %lf\n", (double)(end.QuadPart - start.QuadPart) / freq.QuadPart);

    return 0;   
}