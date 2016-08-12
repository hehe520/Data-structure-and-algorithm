#include <stdio.h>
#include <math.h>
#include <string.h>

void show(int (*map)[16], int size)
{
	int i, j;
	for (i=0; i<size; i++)
	{
		for (j=0; j<size; j++)
		{
			if (map[i][j])
				printf("%3d", map[i][j]);
			else printf("%3c", '*');
		}
		printf("\n\n");
	}
}

static int c = 1;		// 棋盘标号用

// 棋盘覆盖, 起点(sx, sy),  特殊点(x, y), 棋盘大小 size * size
void chess(int (*map)[16], int sx, int sy, int x, int y, int size)
{
	int mid;
	int num;
	
	if (size == 1) return ;	// 棋盘为 1 终止递归
	else 
	{
		num = c++;
		mid = size / 2;
		if (x < sx+mid && y < sy+mid)		// 在左上角
			chess(map, sx, sy, x, y, mid);
		else	// 不在左上角, 左上角的右下角已被涂黑
		{
			map[sx+mid-1][sy+mid-1] = num;
			chess(map, sx, sy, sx+mid-1, sy+mid-1, mid);
		}
		if (x < sx+mid && y >= sy+mid)	// 在右上角
			chess(map, sx, sy+mid, x, y, mid);
		else 
		{
			map[sx+mid-1][sy+mid] = num;
			chess(map, sx, sy+mid, mid-1, mid, mid);
		}
		if (x >= sx+mid && y < sy+mid)	// 在左下角
			chess(map, sx+mid, sy, x, y, mid);
		else
		{
			map[sx+mid][sy+mid-1] = num;
			chess(map, sx+mid, sy, mid, mid-1, mid);
		}
		if (x >= sx+mid && y >= sy+mid)	// 在右下角
			chess(map, sx+mid, sy+mid, x, y, mid);
		else
		{
			map[sx+mid][sy+mid] = num;
			chess(map, sx+mid, sy+mid, sx+mid, sy+mid, mid);
		}
	}
}

int main(void)
{
	int k;
	int map[16][16];
	int x, y;
	int size;

	memset(map, 0, sizeof(map));
	printf("棋盘覆盖问题, 棋盘大小 2^k * 2^k, k = [1..4]\n");
	printf("请输入 k: ");
	scanf("%d", &k);
	size = (int)pow(2, k);
	printf("请输入特殊方格坐标: ");
	scanf("%d %d", &x, &y);
	chess(map, 0, 0, x, y, size);
	show(map, size);

	return 0;
}