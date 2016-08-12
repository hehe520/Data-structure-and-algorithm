// 树的孩子兄弟储存

#include <stdio.h>
#include <malloc.h>

typedef char elemtype;
typedef int status;

#define OK 1
#define ERROR 0
#define SIZE 100	// 队列大小

/*
 /  是首孩子
 -> 是兄弟

      R
     /
    A -> B -> C
   /	     /
  D -> E    F
           /
          G -> H -> K
*/

// 孩子结点结构，类似二叉链表
typedef struct node
{
	elemtype data;		// 结点值
	node *firstchild;	// 孩子老大
	node *nextbrother;	// 孩子兄弟
	node *parent;		// 双亲
}node, cstree;


// 层次建树
status create(cstree *tree)
{
	elemtype c;
	node *queue[50];	// 简单队列
	node *back;			// 出队的元素
	node *n;			// 新结点
	node *cur;			// 遍历用
	int rear = 0;
	int front = 0;

	// 初始化根结点
	printf("请输入根结点：");
	c = getchar();
	fflush(stdin);
	tree->data = c;
	tree->parent = NULL;		// 根结点无双亲
	tree->nextbrother = NULL;	// 根只有一个
	tree->firstchild = NULL;
	// 队列记录下根结点
	queue[rear] = tree;
	rear = (rear + 1) % 100;

	while (rear != front)
	{
		// 此时 back 是双亲
		back = queue[front];
		front = (front + 1) % 50;
		printf("请输入 %c 的孩子：", back->data);
		c = getchar();
		while (c != '\n')
		{
			// 读入c建立孩子结点
			n = (node *)malloc(sizeof(node));
			if (!n) return ERROR;
			n->data = c;
			n->firstchild = NULL;
			n->nextbrother = NULL;
			n->parent = back;

			// n 入队记录
			if ((rear + 1) % 50 == front) return ERROR;
			queue[rear] = n;
			rear = (rear + 1) % 50;

			// 从firstchild走到最后一个孩子的末尾，连上
			cur = back->firstchild;
			if (cur)
			{
				// 沿着兄弟找到最后
				while (cur->nextbrother)
					cur = cur->nextbrother;
				cur->nextbrother = n;
			}
			else back->firstchild = n;
			c = getchar();
		}
	}
	return OK;
}

// 释放树
// 类似二叉树的释放
// firstchild 看作 lchild
// nextbrother 看作 rchild
// 必须是后序释放，不然就找不到孩子了
status destory(cstree *tree)
{
	if (tree)
	{
		destory(tree->firstchild);
		destory(tree->nextbrother);
		free(tree);
		return OK;
	}
	return ERROR;
}

// 先序遍历 (非递归)
// 用栈记录路径
status first_tra_s(cstree *tree, status (*visit)(node *n))
{
	node *stack[SIZE];
	int top = 0;

	while (top || tree)
	{
		// 把首孩子全部入栈
		while (tree)
		{
			visit(tree);
			stack[top++] = tree;
			tree = tree->firstchild;
		}
		// 此时走到左孩子尽头
		// 然后往回退一格
		if (top)
		{
			tree = stack[--top];
			// 往兄弟走一格
			tree = tree->nextbrother;
		}
	}

	return OK;
}

// 先序遍历 (递归)
// 先访问左孩子，再向右访问兄弟
// 对于中序，后序，只需调整访问顺序即可
status first_tra(cstree *tree, status (*visit)(node *n))
{
	if (tree)
	{
		visit(tree);
		first_tra(tree->firstchild, visit);
		first_tra(tree->nextbrother, visit);
		return OK;
	}
	return ERROR;
}

// 层次遍历
// 用队列记录每层结点的首孩子
status BFS_tree(cstree *tree, status (*visit)(node *n))
{
	node *queue[SIZE];
	node *back;
	int rear = 0;
	int front = 0;

	// 首先根结点入队
	queue[rear] = tree;
	rear = (rear + 1) % SIZE;
	while (rear != front)
	{
		// 出来一个结点,遍历其兄弟,记录每个兄弟的首孩子
		back = queue[front];
		front = (front + 1) % SIZE;

		while (back)
		{
			visit(back);
			// 如果有孩子，则记录
			if (back->firstchild)
			{
				queue[rear] = back->firstchild;
				rear = (rear + 1) % SIZE;
			}
			back = back->nextbrother;
		}
	}
	return OK;
}

status show(node *tree)
{
	if (tree)
		printf("%c, ", tree->data);
	return OK;
}

// 查找有没有key的结点, 返回地址
// 可以用广搜查找，深搜查找
// 基本项：
// 如果是空树 返回 NULL
// 如果是 key 值，返回 tree
// 归纳项 继续搜索孩子
// 最后返回 NULL
node *DFS_find(cstree *tree, elemtype *key)
{
	node *addr = NULL;

	if (!tree) return NULL;
	if (tree->data == *key) return tree;

	// 搜索首孩子, 类似二叉树的左孩子
	addr = DFS_find(tree->firstchild, key);
	if (addr) return addr;

	// 搜索兄弟
	addr = DFS_find(tree->nextbrother, key);
	if (addr) return addr;

	// 如果上面都没有找到
	return addr;
}

// 广搜查找结点
// 每次从队列里拿出一个，看看是不是key
node *BFS_find(cstree *tree, elemtype *key)
{
	node *queue[100];
	int rear = 0;
	int front = 0;
	node *back;

	// 根入队
	queue[rear] = tree;
	rear = (rear + 1) % 100;

	while (rear != front)
	{
		back = queue[front];
		front = (front + 1) % 100;
		// 队列出来的只是首孩子
		// 不是首孩子的结点不会进队列
		// 所以这里不会全部遍历

		// 遍历 back 的兄弟，记录下兄弟的首孩子
		while (back)
		{
			// 看看是不是 key
			if (back->data == *key) return back;

			// 记录首孩子
			if (back->firstchild) 
			{
				if ((rear + 1) % 100 == front) return NULL;
				queue[rear] = back->firstchild;
				rear = (rear + 1) % 100;
			}
			// 继续搜索兄弟
			back = back->nextbrother;
		}
	}
	return NULL;
}

// 更新key结点的值,为data
status update(cstree *tree, elemtype *key, elemtype *data)
{
	node *addr = NULL;

	addr = DFS_find(tree, key);
	if (!addr) return ERROR;
	addr->data = *data;
	return OK;
};

// 在pos下面插入一个data结点
status insert(cstree *tree, elemtype *key, elemtype *data, int flag)
{
	node *addr;
	node *n;
	node *cur;

	// 查找 key
	addr = DFS_find(tree, key);
	if (!addr) return ERROR;

	// 建立新结点
	n = (node *)malloc(sizeof(node));
	if (!n) return ERROR;
	n->data = *data;
	n->parent = addr;
	n->firstchild = NULL;
	n->nextbrother = NULL;

	// 准备插入
	if (flag)
	{
		// 插在末尾
		cur = addr->firstchild;
		if (cur)
		{
			while (cur->nextbrother)
				cur = cur->nextbrother;
			cur->nextbrother = n;
			// n->nextbrether 已经被 NULL
		}
		else addr->firstchild = n;
	}
	else 
	{
		// 插在首位, 算法见顶上的图
		// 先把原来的首孩子记录
		n->nextbrother = addr->firstchild;
		// 此时首孩子是 n
		addr->firstchild = n;
		// 如果有兄弟
		if (n->nextbrother)
		{
			// 由 n 继承 a 的首孩子
			n->firstchild = n->nextbrother->firstchild;
			// 抹掉 a 的首孩子
			n->nextbrother->firstchild = NULL;
		}
	}
	return OK;
}

// 根据X结点地址, 返回根结点地址
node *root(node *n)
{
	if (!n) return NULL;
	while (n->parent) n = n->parent;
	return n;
}

// 删除一个叶子或树枝
status remove(cstree *tree, elemtype *data)
{
	node *addr;
	node *cur;
	
	addr = DFS_find(tree, data);
	if (addr)
	{
		// 如果是根，
		if (addr == root(addr)) 
		{
			destory(tree->firstchild);
			tree->firstchild = NULL;
			tree->data = '\0';
		}
		else 
		{
			// 找到 addr 的前一个, 置空指针
			if (addr->parent->firstchild == addr)
				addr->parent->firstchild = addr->nextbrother;
			else 
			{
				cur = addr->parent->firstchild;
				while (cur->nextbrother != addr)
					cur = cur->nextbrother;
				// 此时 cur 的下一个是 addr
				cur->nextbrother = addr->nextbrother;
			}
			destory(addr->firstchild);
			free(addr);
		}
		return OK;
	}
	else return ERROR;
}

status create2(cstree *tree, char *str)
{
	node *queue[50];
	int rear = 0;
	int front = 0;
	node *back;
	node *n;
	node *last;		// 尾指针

	if (str[0] == '#' || str[0] == '\0') return ERROR;
	// 建立头结点，并入队
	tree->data = *str++;
	tree->parent = NULL;
	tree->firstchild = NULL;
	tree->nextbrother = NULL;
	queue[rear] = tree;
	rear = (rear + 1) % 50;

	while (rear != front)
	{
		back = queue[front];
		front = (front + 1) % 50;

		while (str[0] != '#' && str[0] != '\0')
		{
			n = (node *)malloc(sizeof(node));
			if (!n) return ERROR;
			n->data = *str++;
			n->parent = back;
			n->firstchild = NULL;
			n->nextbrother = NULL;
			// 连上 back
			if (back->firstchild)
				last->nextbrother = n;
			else 
				back->firstchild = n;
			last = n;
			// 入队记录
			if ((rear + 1) % 50 == front) return ERROR;
			queue[rear] = n;
			rear = rear + 1;
		}
		if (str[0] == '\0') break;
		else str++;
	}
	return OK;
}

int main(void)
{
	cstree tree;
	elemtype data;
	elemtype key;
	node *addr;
	node *r;
	int flag;
	char str[100] = {0};

	create(&tree);
//  RABC#DE##F###GHK###
//	scanf("%s", &str);
//	getchar();
//	create2(&tree, str);   // 根据字符串建树
	printf("\n先序遍历：");
	first_tra(&tree, show);

	printf("\n\n");
	printf("层次遍历：");
	BFS_tree(&tree, show);	

	printf("\n\n请输入要查找的结点值: ");
	scanf("%c", &data);
	addr = DFS_find(&tree, &data);   // 深搜
//	addr = BFS_find(&tree, &data);   // 广搜也可
	if (addr)
	{
		printf("找到：地址为: %x\n", addr);
		printf("结点为：%c\n", addr->data);
		if (addr->parent)
			printf("双亲为：%c\n", addr->parent->data);
		r = root(addr);
		printf("树根为：%c\n", r->data);
	}
	else printf("找不到 %c\n", data);

	printf("\n删除一个结点或叶子：");
	fflush(stdin);
	scanf("%c", &key);
	if (remove(&tree, &key))
	{
		printf("删除成功\n");
		printf("先序--栈：");
		first_tra_s(&tree, show);
		printf("\n\n");
	}
	else printf("删除失败，找不到 %c\n", key);

	// A 下插入 X, 0表示插在首位, 1是末尾
	printf("\n请输入要插入的结点:(如 A X 1) ");
	fflush(stdin);
	scanf("%c %c %d", &key, &data, &flag);
	if (insert(&tree, &key, &data, flag))
	{
		printf("插入成功\n");
		printf("\n先序--栈：");
		first_tra_s(&tree, show);
		printf("\n");
	}
	else printf("插入失败\n");
	destory(tree.firstchild);
	tree.firstchild = NULL;

	return 0;
}