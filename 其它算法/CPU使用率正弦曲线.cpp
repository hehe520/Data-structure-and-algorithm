
// 让 CPU 使用率成正弦曲线
// 注意在多核机器下, 要设定一个 CPU 运行该程序

// sin() 函数使用说明, 高中数学几个特殊的 sin 值 sin(PI/6) = 1/2, sin(PI/2) = 1
// sin(x), x 范围在 [0, 2 * PI]
// 将 [0, 2 * PI] 这个区间切割成 200 份, 每份对应一个点
// 每 250 MS 绘制一个点, 1 秒正好画 4 个点
// 任务管理器 CPU 曲线 1 秒刷新一次, 画完 200 个点 50 秒

#include <stdio.h>
#include <math.h>
#include <windows.h>
#include <time.h>

const double P = 3.1514926536;
const int    NUM = 200;			// 200 个点
const int    TIMES = 250;		// 时间片 250 ms
const int    F = TIMES / 2;		// 振幅
const double inc = 2.0 / NUM;	// sin(x) 的 x 增量

int main(void)
{
	DWORD st;		// 开始时间
	DWORD busytime[NUM];	// 繁忙时间
	int i;
	double rad;

	// 将本线程锁定在第一个 CPU 上
	// 0x0001 二进制中的 1 表示使用哪个CPU
	SetThreadAffinityMask(GetCurrentThread(), 0x0001);

	// 生成正弦曲线的 busytime
	for (rad=i=0; i<NUM; i++)
	{
		busytime[i] = (DWORD)(sin(P * rad) * F + F);
		// sin(P * rad) 产生 [-1.0, 1.0] 的浮点数
		// sin(P * rad) * F 产生振幅增量
		// 最后加上 F 就把 [-1.0, 1.0] 变换到 [0, 250] 区间
		rad += inc;
	}
	// 循环画图
	for (i=0; ; i = (i + 1) % NUM)
	{
		st = clock();
		// 忙时
		while (clock() - st <= busytime[i]);
		// 闲时, 加起来等于 TIMES
		Sleep(TIMES - busytime[i]);
	}
	return 0;
}