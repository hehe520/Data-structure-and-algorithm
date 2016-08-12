#include <stdio.h>
#include <malloc.h>

void show(int (*map)[7])
{
	int i, j;

	for (i=0; i<7; i++)
	{
		for (j=0; j<7; j++)
			printf("%3d", map[i][j]);
		printf("\n");
	}
}

// map保存到 p 中
void encode(int **p, int (*map)[7])
{
	int i, j;
	int cur = 0;

	for (i=0; i<7; i++)
		for (j=0; j<=i; j++)
			(*p)[cur++] = map[i][j];
}

// p 中的元素还原到 map 中
void decode(int *p, int (*map)[7])
{
	int i, j;
	int cur = 0;

	for (i=0; i<7; i++)
		for (j=0; j<=i; j++)
			map[j][i] = map[i][j] = p[cur++];
}

// 定位函数
inline int find(int i, int j)
{
	return i*(i-1)/2 + j - 1;
}

void show_tri(int *p)
{
	int i, j;
	int addr;

	for (i=1; i<=7; i++)
	{
		for (j=1; j<=7; j++)
		{
			addr = find(i, j);
			printf("%3d", p[addr]);
		}
		printf("\n");
	}
}

int main(void)
{
	int map[7][7] = 
	{
		0, 7, 0, 6, 0, 0, 3,
		7, 2, 0, 0, 0, 9, 4,
		0, 0, 0, 0, 4, 0, 0,
		6, 0, 0, 0, 0, 0, 0,
		0, 0, 4, 0, 0, 0, 7,
		0, 9, 0, 0, 0, 0, 0,
		3, 4, 0, 0, 7, 0, 8,
	};
	int buf[7][7] = {0};	// 恢复矩阵
	int *p = NULL;		// 保存下三角矩阵

	printf("原矩阵：\n");
	show(map);
	p = (int *)malloc(30 * sizeof(int));
	encode(&p, map);
	decode(p, buf);
	printf("\n压缩到数组还原后：\n");
	show(buf);
	printf("\n直接在数组中输出：\n");
	show_tri(p);
	printf("\n");
	free(p);

	return 0;
}
