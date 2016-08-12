#include <stdio.h>
#include <malloc.h>

typedef int status;
typedef int elemtype;

#define OK 1
#define ERROR 0

int size;   // 统计压缩后的大小

// 十字链表结点
typedef struct node
{
	int x, y;		// 记录坐标
	elemtype data;	// 值
	node *right;
	node *down;
}node;

// 行列专用的结点，省空间
typedef struct hnode
{
	node *first;	// 指向上面的结点
}hnode;

// 头结构
typedef struct head
{
	hnode **row;   // 动态管理行
	hnode **col;   // 动态管理列
	int r, c, all;   // 行数，列数，结点总数
}head;

// 初始化十字链表
status create(head *p, int x, int y)
{
	int i;

	if (!p) return ERROR;
	
	// 开始建立行，列的指针数组
	p->row = (hnode **)malloc(x * sizeof(hnode));
	if (!p->row) return ERROR;
	p->col = (hnode **)malloc(y * sizeof(hnode));
	if (!p->col) return ERROR;
	p->r = x;
	p->c = y;

	// 开始给行，列指针分配空间, 并初始化
	for (i=0; i<x; i++)
	{
		p->row[i] = (hnode *)malloc(sizeof(node));
		if (!p->col[i]) return ERROR;
		p->row[i]->first = NULL;
	}
	for (i=0; i<y; i++)
	{
		p->col[i] = (hnode *)malloc(sizeof(node));
		if (!p->col[i]) return ERROR;
		p->col[i]->first = NULL;
	}
	p->all = 0;

	size += (x + y) * (sizeof(hnode) + sizeof(node));
	return OK;
}

// 释放十字链表
status destory(head *p, int x, int y)
{
	int i;
	node *cur;

	// 释放全部结点
	for (i=0; i<x; i++)
	{
		cur = p->row[i]->first;
		while (cur)
		{
			p->row[i]->first = cur->right;
			free(cur);
			cur = p->row[i]->first;
		}
	}
	// 释放行列指针数组
	if (!p->col) free(p->col);
	if (!p->row) free(p->row);
	
	return OK;
}

// 交换两个结点的值域
status swap(node *a, node *b)
{
	node temp;

	temp.data = a->data;
	temp.x = a->x;
	temp.y = a->y;
	a->data = b->data;
	a->x = b->x;
	a->y = b->y;
	b->data = temp.data;
	b->x = temp.x;
	b->y = temp.y;
	return OK;
}

// 把 map 压缩到十字链表 p 中
status encode(head *p, int map[][7], int x, int y)
{
	int i, j;
	hnode *hang;
	hnode *lie;
	node *cur;
	node *n;

	if (!(p->col && p->row)) return ERROR;
	for (i=0; i<x; i++)
	{
		for (j=0; j<y; j++)
		{
			if (map[i][j])
			{
				n = (node *)malloc(sizeof(node));
				if (!n) return ERROR;
				size += sizeof(node);
				n->x = i;
				n->y = j;
				n->data = map[i][j];
				n->right = n->down = NULL;

				// 插入十字链表
				// 寻找行, 那就比较列的大小
				hang = p->row[i];
				cur = hang->first;
				if (!cur) hang->first = n;
				else 
				{
					while (cur->right)
					{
						if (j >= cur->y)
							break;
						cur = cur->right;
					}
					n->right = cur->right;
					cur->right = n;
					swap(cur, n);	// 插在后面，交换数值
				}
	
				// 寻找列，那就比较行的大小
				lie = p->col[j];
				cur = lie->first;
				if (!cur) lie->first = n;
				else 
				{
					while (cur->down)
					{
						if (i >= cur->x)
							break;
						cur = cur->down;
					}
					n->down = cur->down;
					cur->down = n;
					swap(cur, n);
				}
				p->all++;
			}
		}
	}
	return OK;
}

// 把十字链表还原到 map 中
status decode(head *p, int map[][7], int x, int y)
{
	node *cur;
	int i;

	if (!(p->col && p->row)) return ERROR;

	// 其实还原就是遍历十字链表
	for (i=0; i<p->r; i++)
	{
		cur = p->row[i]->first;
		while (cur)
		{
			map[ cur->x ][ cur->y ] = cur->data;
			cur = cur->right;
		}
	}
	return OK;
}

void show(int m[][7])
{
	int i, j;

	for (i=0; i<6; i++)
	{
		for (j=0; j<7; j++)
			printf("%4d", m[i][j]);
		printf("\n");
	}
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
	head list;
	int buf[6][7] = {0};
	int resize;

	size = 0;
	resize = sizeof(map);
	printf("原矩阵：\n");
	show(map);
	printf("\n十字链表压缩还原后：\n\n");
	create(&list, 6, 7);

	encode(&list, map, 6, 7);   // map 压缩到 list 中
	decode(&list, buf, 6, 7);   // list 还原到 buf 中
	show(buf);
	printf("\n原大小：%d 字节, 压缩后大小：%d 字节\n压缩率：%.2f%%\n",
		resize, size, 100*(size / (float)resize));
	destory(&list, 6, 7);

	// 结果越压越大，因为结点的值域太小(才4个字节)
	// 提高结点的值域可以达到压缩的效果
	// 故十字链表适合值域较大的数据压缩储存

	return 0;
}
