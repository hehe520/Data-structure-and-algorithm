/* ---------------------------------
// 树的等价问题 - 并查集
// 主要函数: 
// init()		每个人都是老大
// find_set()	查找你老大是谁
// merge()		合并两个帮派
// 实现查找老大函数有三个
// find_set()    递归压缩
// find_set_s()  非递归, 借助栈
// find_set_y()  非递归, 复杂度最小
-----------------------------------*/

#include <stdio.h>
#include <malloc.h>

typedef char elemtype;
#define SIZE 10		// 固定大小

// 树采用双亲法表示
typedef struct
{
	int parent;		// 指向双亲
	int depth;		// 深度
	elemtype data;
}node;

// 固定大小为 10
typedef struct
{
	node table[SIZE];
//	int cur;   可以加一个表示使用量
}forest;

forest f;		// 定义一个全局的森林, 因为多个函数要用到

// 初始化结点, 每个结点都是根, 没有双亲
// 即: 每个结点自己成一棵树, 单独一个集合
// 双亲表示法中 -1 表示没有双亲
// 也可指向自己 (x.parent == x) 表示没有双亲
void init(forest *t)
{
	int i;

	// 有 {A}{B}{C}{D}{E}{F}{G}{H}{J}{I} 这么多树
	for (i=0; i<SIZE; i++)
	{
		t->table[i].data = 'A' + i;
		t->table[i].parent = i;
		t->table[i].depth = 0;
	}
}

// 查找集合的时候, 顺便递归压缩路径
// 如果 addr的parent == addr 则返回 addr
// 否则 把当前结点的双亲 改为 双亲所在的根
// 递归语句可看成 
// temp = find_set(...parent);
// parent = temp   所以是在回溯时, 修改双亲的
int find_set(int addr)
{
	if (f.table[addr].parent != addr)
	{
		f.table[addr].parent = find_set( f.table[addr].parent );
		f.table[addr].depth = 1;	// 压缩后深度也变了
	}
	return f.table[addr].parent;
	// 这里不能返回addr, 因为 addr 没有被修改
}

// 非递归压缩路径
// 也就是自己用栈记录, 对于大量数据应采用非递归的
int find_set_s(int addr)
{
	int stack[11];		// 辅助空间 11
	int top = 0;
	int tmp;

	while (f.table[addr].parent != addr)
	{
		stack[top++] = addr;
		addr = f.table[addr].parent;	// 类似 p = p->next;
	}
	while (top)		// 全部出栈, 压缩路径
	{
		tmp = stack[--top];
		f.table[tmp].parent = addr;  // 直接连接到根节点下
		f.table[tmp].depth = 1;
	}
	return addr;
}

// 非递归压缩路径, 此算法空间复杂度更低
int find_set_y(const int addr)
{
	int i = addr;
	int tmp = addr;

	while (f.table[i].parent != i)	
		i = f.table[i].parent;

	// 此时 i 是根了, 再压缩一下路径
	while (f.table[tmp].parent != i)
	{
		f.table[tmp].parent = i;
		f.table[tmp].depth = 1;
	}
	return i;
}

// 合并 {a}, {b} 两个集合
void merge(int a, int b)
{
	int a_root, b_root;

	a_root = find_set_y(a);		// 查找所在的根
	b_root = find_set_y(b);		// find_set_s() 也测试通过
	
	if (a_root != b_root)		// 如果 a, b 不是同一个集合
	{
		// a 所在树的深度小, 使得深度最小
		if (f.table[a].depth < f.table[b].depth)
		{
			f.table[a_root].parent = b_root;   // 把a的根并过去
		}
		else if (f.table[a].depth > f.table[b].depth)
		{
			f.table[b_root].parent = a_root;
		}
		else 
		{
			f.table[b_root].parent = a_root;
			f.table[a_root].depth++;
		}
	}
}

void show_root(forest *f)		// 查看双亲结点
{
	int i;
	printf("根结点: ");
	for (i=0; i<10; i++)
		printf("%d, ", find_set_y(i));
	printf("\n");
}

int main(void)
{
	int i;

	init(&f);	// 初始化森林

	// 一开始有 {A}{B}{C}{D}{E}{F}{G}{H}{J}{I} 这么多集合
	show_root(&f);
	
	// 两两合并后 {A,B} {C,D} {E,F} {G,H} {J,I}
	for (i=0; i<10; i+=2) merge(i, i + 1);
	show_root(&f);

	// 继续合并 {A,B,C,D} {E,F,G,H} {J,I}
	merge(0, 2);   merge(4, 6);
	show_root(&f);

	// 继续合并 {A,B,C,D,E,F,G,H,J,I} 一个集合了
	merge(3, 7);   merge(7, 9);
	show_root(&f);

	return 0;
}
