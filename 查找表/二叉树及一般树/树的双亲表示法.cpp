// 树的双亲表示法

#include <stdio.h>
#include <malloc.h>

typedef char elemtype;
typedef int status;

#define ERROR 0
#define OK 1
#define SIZE 50

typedef struct pnode
{
	elemtype data;
	int parent;
}pnode;

typedef struct ptree
{
	pnode *base;	// 数组基址
	int cur;
	int size;
}ptree;

status create(ptree *tree)
{
	int queue[SIZE] = {0};   // 临时队列
	int rear = 0; 
	int front = 0;
	int cur = 0;	// 层次建树时，指向正在建树的结点
	char c;

	// 初始化 tree
	tree->base = (pnode *)malloc(SIZE * sizeof(pnode));
	if (!tree->base) return ERROR;
	tree->size = SIZE;
	tree->cur = 0;
	printf("请输入根结点：");
	c = getchar();
	fflush(stdin);
	
	// 建立根结点
	tree->base[0].data = c;
	tree->base[0].parent = -1;
	tree->cur = 1;
	// 根结点入队
	queue[rear] = cur;
	rear = (rear + 1) % SIZE;

	while (rear != front)
	{
		cur = queue[front];		// 从队列里拿出要建孩子的结点
		front = (front + 1) % SIZE;
		printf("请输入 %c 的孩子：", tree->base[cur].data);
		c = getchar();
		while (c != '\n')
		{
			tree->base[tree->cur].data = c;
			tree->base[tree->cur].parent = cur;
			// 孩子全部入队记录
			if ((rear + 1) % SIZE != front)
			{
				queue[rear] = tree->cur;
				rear = (rear + 1) % SIZE;
			}
			else return ERROR;
			c = getchar();
			tree->cur++;
		}
		cur++;		// 遍历数组
	}
	return OK;
}

status destory(ptree *tree)
{
	if (tree->base) free(tree->base);
	tree->base = NULL;
	return OK;
}

// 寻找 c 结点的地址
pnode *find(ptree *tree, elemtype c)
{
	int i;

	// 遍历表的 data 项
	for (i=0; i<tree->cur; i++)
		if (tree->base[i].data == c)
			return tree->base + i;
	return NULL;
}

// 返回 c 的双亲
elemtype parents(ptree *tree, elemtype c)
{
	pnode *addr;
	int p;

	addr = find(tree, c);
	if (!addr) return '\0';
	// addr - tree->base 地址之差，是位序之差
	p = tree->base[addr - tree->base].parent;
	return tree->base[p].data;
}

// 返回 c 的根
elemtype roots(ptree *tree, elemtype c)
{
	pnode *addr;
	int p;

	addr = find(tree, c);
	if (!addr) return '\0';
	p = tree->base[addr - tree->base].parent;
	while (p != -1)
	{
		// p = p的双亲, 记录地址
		addr = tree->base + p;
		p = tree->base[p].parent;
	}
	return addr->data;
}

// 插入结点
status insert(ptree *tree, elemtype c, elemtype data)
{
	pnode *addr;

	// 看看表满了没
	if (tree->cur == tree->size)
	{
		tree->size += 10;
		tree->base = (pnode *)realloc(tree->base, 
			tree->size * sizeof(pnode));
		if (!tree->base) return ERROR;
	}
	addr = find(tree, c);
	if (!addr) return ERROR;
	tree->base[tree->cur].data = data;
	tree->base[tree->cur].parent = addr - tree->base;
	tree->cur++;

	return OK;
}

int main(void)
{
	ptree tree;
	pnode *addr = NULL;
	elemtype c;
	elemtype data;

	printf("按回车键表示输入结束\n");
	create(&tree);
	printf("建立完毕！\n");
	printf("查找结点：");
	c = getchar();
	addr = find(&tree, c);
	if (addr)
	{
		printf("\n找到 %c，地址为：%x\n", addr->data, addr);
		printf("双亲为：%c\n", parents(&tree, c));
		printf("根结点为：%c\n", roots(&tree, c));
	}
	else printf("没有找到!\n");

	printf("在X下插入Z结点：(如:X Z)");
	c = getchar();
	data = getchar();
	fflush(stdin);
	insert(&tree, c, data);

	destory(&tree);

	return 0;
}