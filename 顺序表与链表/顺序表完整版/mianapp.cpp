#include "sqlist.h"

sqlist list;

int main(void)
{
	init(&list);  // 初始化表
	display();    // 显示界面
	wait();       // 等待用户操作

	return 0;
}
