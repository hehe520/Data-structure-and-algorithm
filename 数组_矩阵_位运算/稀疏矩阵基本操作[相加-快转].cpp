// 包含，三元组储存，矩阵相加，快速转置等等

#include <stdio.h>
#include <malloc.h>
#include <string.h>

typedef int elemtype;
typedef int status;

#define ERROR 0
#define OK 1

// 三元表储存结构
typedef struct
{
	int x;
	int y;
	elemtype data;
}unit;

// 三元表头结构
typedef struct
{
	unit *base;	// 基址
	int lenth;	// 长度
}array;

// 初始化表
status init(array *s, int x, int y)
{
	s->base = (unit *)malloc(sizeof(unit));
	if (!s->base) return ERROR;
	// 0 号表项用来记录矩阵的行列
	s->base->x = x;		// 行数
	s->base->y = y;		// 列数
	s->base->data = 0;	// 非零元数
	s->lenth = 1;

	return OK;
}

status destory(array *s)
{
	if (s->base) free(s->base);
	s->base = NULL;
	s->lenth = 0;

	return OK;
}

// 输出矩阵
void show(int map[][7], int x, int y)
{
	int i, j;

	for (i=0; i<x; i++)
	{
		for (j=0; j<y; j++)
		{
			printf("%4d", map[i][j]);
		}
		printf("\n");
	}
	return ;
}

// 追加data表项
status append(array *s, int x, int y, elemtype data)
{
	// 检查 s 表
	if (!s->base) return ERROR;
	s->base = (unit *)realloc(s->base, 
		(s->lenth + 1) * sizeof(unit));
	if (!s->base) return ERROR;
	s->base[s->lenth].x = x;
	s->base[s->lenth].y = y;
	s->base[s->lenth].data = data;
	s->lenth++;

	return OK;
}

// 压缩矩阵
void encode(array *s, int x, int y, int map[][7])
{
	int i, j;

	// map 矩阵压缩到 list 中..
	for (i=0; i<x; i++)
		for (j=0; j<y; j++)
			// 如果不是 0 就追加到表中
			if (map[i][j]) append(s, i, j, map[i][j]);
}

// 还原矩阵
void decode(array *s, int x, int y, int buf[][7])
{
	int i;
	for (i=1; i<s->lenth; i++)
	{
		buf[ s->base[i].x ][ s->base[i].y ]
			= s->base[i].data;
	}
}

// 矩阵相加
status add(array *a, array *b, array *c)
{
	int ia = 1;
	int ib = 1;
	int ax, bx, ay, by;
	elemtype data;

	// 比较 a b 表的长宽, 大小一样才可相加
	if (!(a->base && b->base)) return ERROR;
	if (a->base[0].x != b->base[0].x 
		|| a->base[0].y != b->base[0].y) return ERROR;
	while (ia < a->lenth && ib < b->lenth)
	{
		ax = a->base[ia].x;
		bx = b->base[ib].x;
		ay = a->base[ia].y;
		by = b->base[ib].y;
		if (ax == bx)
		{
			if (ay == by)
			{
				// 相加后，追加到 c 中
				data = a->base[ia].data + b->base[ib].data;
				append(c, ax, ay, data);
				ia++;
				ib++;
			}
			else 
			{
				if (ay < by) 
					append(c, ax, ay, a->base[ia++].data);
				else
					append(c, bx, by, b->base[ib++].data);
			}
		}
		else
		{
			if (ax < bx)
				append(c, ax, ay, a->base[ia++].data);
			else
				append(c, bx, by, b->base[ib++].data);
		}
	}
	return OK;
}

// 矩阵转置
status transpose(array *a, array *b)
{
	int *num;	// 第x列非0元个数
	int *pos;	// 在b表中的适合位置
	int i;
	int posb;

	// a  x  y  data
	//    1  2   12
	//    1  3   39
	//    3  1   11
	// 统计非 0 元个数
	num = (int *)malloc(a->lenth * sizeof(int));
	if (!num) return ERROR;
	memset(num, 0, (a->lenth+1)*sizeof(int));
	for (i=1; i<a->lenth; i++)
	{
		num[a->base[i].y]++;	// 统计每一列非0元
	}

	pos = (int *)malloc(a->lenth * sizeof(int));
	if (!pos) return ERROR;
	pos[0] = 1;
	for (i=0; i<a->lenth; i++)
	{
		// 计算位置
		pos[i+1] = pos[i] + num[i];
	}

	for (i=1; i<a->lenth; i++)
	{
		// a中第i个元素, 在b中的pos[i]位置上
		// x y 位置要对调
		posb = pos[ a->base[i].y ];
		b->base[posb].x = a->base[i].y;
		b->base[posb].y = a->base[i].x;
		b->base[posb].data = a->base[i].data;
		b->lenth++;
		pos[ a->base[i].y ]++;	// 书上有解释
	}
	b->base[0].x = a->base[0].y;
	b->base[0].y = a->base[0].x;

	return OK;
}

int main(void)
{
	int map[6][7] = 
	{
		0, 12, 39,  0, 0,  0,  0,
		0,  0,  0,  0, 0,  0,  0,
		11, 0,  0,  0, 0, 14,  0,
		0,  0, 24,  0, 0,  0,  0,
		0, 18,  0,  0, 0,  0,  0,
		15, 0,  0, 15, 0,  0,  0,
	};				// 手工输入
	int map2[6][7] = 
	{
		12, 0, 39,  1, 2,  3,  4,
		0,  0,  0,  0, 0,  0,  0,
		11, 0,  0,  0, 0, 14,  0,
		0,  24, 0,  0, 0,  0,  0,
		18, 2,  0,  0, 0,  0,  0,
		15, 0,  0, 15, 0,  0,  0,
	};				// 手工输入
	array list;		// 三元顺序表
	array listbuf;
	array list3;
	array listmap;
	int buf[10][7] = {0};
	int buf3[6][7] = {0};
	float resize;	// 原大小
	float newsize;	// 压缩后大小
	
	init(&list, 6, 7);	// 初始 6 行 7 列
	init(&listbuf, 6, 7);
	init(&list3, 6, 7);
	init(&listmap, 6, 7);
	printf("原矩阵\n");
	show(map, 6, 7);
	encode(&list, 6, 7, map);

	resize = (float)sizeof(map);
	newsize = (float)sizeof(list) + list.lenth * sizeof(unit);
	printf("压缩率：%.2f%%\n\n", 100*(1 - newsize / resize));

	// 把 list 还原到 buf 中
	decode(&list, 6, 7, buf);
	printf("还原后\n");
	show(buf, 6, 7);
	encode(&listbuf, 6, 7, buf);

	// 矩阵相加
	encode(&listmap, 6, 7, map2);
	add(&listmap, &listbuf, &list3);
	decode(&list3, 6, 7, buf3);
	printf("\n相加后\n");
	show(buf3, 6, 7);

	// 矩阵转置
	destory(&listbuf);
	init(&listbuf, 7, 6);
	memset(buf, 0, sizeof(buf));
	printf("转置后\n");
	transpose(&list, &listbuf);
	decode(&listbuf, 7, 6, buf);
	show(buf, 7, 6);
	
	destory(&list);
//	destory(&listbuf);
	destory(&list3);
//	destory(&listmap);
	
	return 0;
}
