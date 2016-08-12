
// <编程之美>, 1.5 找出机器故障
// 问题描述: 机器 A 有 n 个数字, n > 100000, 机器 B 是机器 A 的备份,
// 现在机器 A 丢失了一个数字, 找出丢失了哪个数字, 数字在 int 的范围
// 问题转化到有一组数字, 每个数字都出现了两次, 只有一个数字是出现一次的
// 找出这个数字

// 解法一: 将机器 B 的数据覆盖到 A 上, 首先数据量很大, 写的费用比读要高
// 解法二: 将所有数字加入哈希表, 找出次数为一次的数字
// 这样做时间 O(n), 空间 O(n)
// 解法三: 在解法二的基础上, 把次数为 2 的数字从表中删除, 空间能小一点
// 解法四: 将机器 B 的数字总和, 减去 A 的数字总和, 即是答案, 
// 这里要用高精度计算, 时空都复杂了

// 解法五: 异或 xor, 已知 a xor a = 0, a xor 0 = b
// 且异或满足交换律 (a xor b) xor c = a xor (b xor c)
// 也满足结合律 a xor b xor c = a xor (b xor c)
// 所以, 一组数字中出现了两次的数字异或后, 都变成 0
// 剩下出现一次的数字异或 0, 还是自身
// 时间 O(n), 空间复杂度降到了 O(1)

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM 500

int main(void)
{
	unsigned int a[NUM];
	unsigned int b[NUM];
	unsigned int pos, ans;
	int i, e;

	for (i=0; i<NUM; i++)
		a[i] = b[i] = i + 1;
	srand(time(NULL));
	pos = rand() % NUM;
	printf("人为丢弃 %d\n\n", a[pos]);
	for (i=pos; i<NUM; i++) a[i] = a[i + 1];

	// 开始异或查找
	ans = a[0];
	e = NUM - 1;
	for (i=1; i<e; i++) ans ^= a[i];
	for (i=0; i<NUM; i++) ans ^= b[i];
	printf("缺少 %d\n\n", ans);

	return 0;
}
