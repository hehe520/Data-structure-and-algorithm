#include <stdio.h>
#include <malloc.h>
#include "Btree.h"

// 基本项：
// 如果是空格,表示不建树
// 如果不是空格，创建一个树结点
// 归纳项：
// 对于 左孩子 也是如此
// 对于 右孩子 也是如此
status create(btnode **tree)
{
	char c;

	c = getchar();
	// 基本项
	if (c == '#') *tree = NULL;
	else if (c != '\n')
	{
		// 直接在 tree 的基础上建立
		(*tree) = (btnode *)malloc(sizeof(btnode));
		if (!(*tree)) return ERROR;
		(*tree)->data = c;
		(*tree)->lchild = NULL;
		(*tree)->rchild = NULL;

		// 归纳项:
		create(&( (*tree)->lchild ));
		create(&( (*tree)->rchild ));
	}
	return OK;
}
// 说明：
// 因为不能建立指针的引用
// 所以只有把tree指针的指针传进来
// 那么 *tree 就是mian函数里面的指针了，不会赋不到值
// 所以 (*tree)->lchild 就是孩子指针
// 那么 &( (*tree)->lchild ) 就是孩子指针的地址了


// 根据str来建树
char *create2(btnode **tree, char *str)
{
	if (*str != '\0')
	{
		if (*str == '#') *tree = NULL;
		else 
		{
			(*tree) = (btnode *)malloc(sizeof(btnode));
			if (!(*tree)) return ERROR;
			(*tree)->data = *str;
			(*tree)->lchild = NULL;
			(*tree)->rchild = NULL;

			// 归纳项
			str = create2(&( (*tree)->lchild ), str + 1);
			// 建完左子树的 str, 加 1 后再传给下面
			str = create2(&( (*tree)->rchild ), str + 1);
			// 建完右子树的 str, 再返回去
		}
	}
	return str;
}


// 销毁树
// 如果不是空指针，先销毁左孩子，后销毁右孩子
// 最后再销毁自己，
// 如果要先销毁自己，则要记录下孩子的地址
// 不然就找不到孩子了
status destory(btnode *tree)
{
	if (tree)
	{
		destory(tree->lchild);
		destory(tree->rchild);
		free(tree);
		tree = NULL;   // 安全性
		return OK;
	}
	return ERROR;
}


// 先序遍历 (递归), 执行 visit 操作
// 基本项
// 如果不是空，则执行visit操作
// 对于左孩子如此
// 对于右孩子也如此
status first_tra(btnode *tree, status (*visit)(btnode *node))
{
	if (tree)  // 如果有树,才会执行visit的,所以visit中不再判断
	{
		visit(tree);
		first_tra(tree->lchild, visit);
		first_tra(tree->rchild, visit);
	}
	else return ERROR;
	return OK;
}
// 书上的算法保险起见
// 所以用了 if (visit(tree))


// 中序遍历 (递归)
// 中序是先访问左孩子，再根结点，最后才是右孩子
status mid_tra(btnode *tree, status (*visit)(btnode *node))
{
	if (tree)
	{
		mid_tra(tree->lchild, visit);
		visit(tree);
		mid_tra(tree->rchild, visit);
	}
	else return ERROR;
	return OK;
}

// 后序遍历 (递归)
// 先访问左孩子，然后右孩子，最后才是根
status last_tra(btnode *tree, status (*visit)(btnode *node))
{
	if (tree)
	{
		last_tra(tree->lchild, visit);
		last_tra(tree->rchild, visit);
		visit(tree);
	}
	else return ERROR;
	return OK;
}

// ------------队列函数--------------
// 把结点的地址入队，而不是整个结点入队
static int front;
static int rear;

status enqueue(btnode *q[SIZE], btnode *data)
{
	if ((rear+1)%SIZE == front) return ERROR;
	q[rear] = data;
	rear = (rear + 1) % SIZE;
	return OK;
}

status dequeue(btnode *q[SIZE], btnode **back)
{
	if (rear == front) return ERROR;
	*back = q[front];
	front = (front + 1) % SIZE;

	return OK;
}

// 层次遍历 (队列广搜)
status wild_tra(btnode *tree, status (*visit)(btnode *node))
{
	btnode *back;
	btnode *q[SIZE];	// 队列

	front = rear = 0;
	if (tree)
	{
		// 根结点入队
		// 把与根结点有关系的也入队
		enqueue(q, tree);
		while (rear != front)
		{
			// 出队，进行visit操作
			// 有关的结点，入队
			dequeue(q, &back);	// back承接结点值域
			visit(back);
			if (back->lchild) enqueue(q, back->lchild);
			if (back->rchild) enqueue(q, back->rchild);
		}
	}
	return OK;
}

// 求树的深度
// 基本项:
// 如果 tree==NULL 深度为 0
// 否则 深度为 1
// 归纳项:
// 深度 = max(左孩子, 右孩子);
int depth(btnode *tree)
{
	int max;
	int left, right;

	if (!tree) return 0;
	else
	{
		left = depth(tree->lchild);
		right = depth(tree->rchild);
		if (left > right) max = left;
		else max = right;

		return max + 1;
	}
}

// 返回结点个数
int nodenum(btnode *tree)
{
	int num = 0;

	if (tree)
	{
		num += nodenum(tree->lchild);
		num += nodenum(tree->rchild);
		return num + 1;
		// 因为自己也算一个，所以是 num+1
	}
	return 0;
}

// 查找有没有key的结点, 返回地址
// 基本项：
// 如果 tree==NULL 返回NULL
// 如果 key==data  返回tree
// 归纳项：
// 对于左孩子，右孩子同样
// 如果找到一个就返回
// 如果都找不到就返回 NULL
btnode *find(btnode *tree, elemtype *key)
{
	btnode *addr = NULL;

	// 基本项:
	if (!tree) return NULL;
	if (tree->data == *key)
		return tree;

	// 归纳项:
	addr = find(tree->lchild, key);
	if (addr) return addr;   // 有才返回

	addr = find(tree->rchild, key);
	if (addr) return addr;

	return addr;
}

// 更新key结点的值，data
status update(btnode *tree, elemtype *key, elemtype *data)
{
	btnode *addr;

	addr = find(tree, key);
	if (!addr) return ERROR;
	addr->data = *data;

	return OK;
}

// 在key下的rl位置插入一个孩子结点data, rl表示左右
status insert(btnode *tree, 
			  elemtype *key, 
			  elemtype *data, 
			  char rl)
{
	btnode *addr;

	addr = find(tree, key);
	if (!addr) return ERROR;
	if (rl == '0')
	{
		if (addr->lchild) return ERROR;
		addr->lchild = (btnode *)malloc(sizeof(btnode));
		if (!addr->lchild) return ERROR;
		addr->lchild->lchild = NULL;
		addr->lchild->rchild = NULL;
		addr->lchild->data = *data;
		return OK;
	}
	if (rl == '1')
	{
		if (addr->rchild) return ERROR;
		addr->rchild = (btnode *)malloc(sizeof(btnode));
		if (!addr->rchild) return ERROR;
		addr->rchild->lchild = NULL;
		addr->rchild->rchild = NULL;
		addr->rchild->data = *data;
		return OK;
	}
	return ERROR;
}

// 删除key下的rl结点
status remove(btnode *tree, elemtype *key, char rl)
{
	btnode *addr;

	addr = find(tree, key);
	if (!addr) return ERROR;
	if (rl == '0')
	{
		destory(addr->lchild);
		addr->lchild = NULL;	// 安全性
	}
	else 
	{
		destory(addr->rchild);
		addr->rchild = NULL;
	}
	return OK;
}

// 求叶子树
// 先序遍历，找到度为 0 的结点
// 直接调用上的遍历函数也可
// 这里再重新写一下
int leafnum(btnode *tree)
{
//	static int sum = 0;	// 这样也可
	int sum = 0;

	if (tree)
	{
		if (!(tree->lchild || tree->rchild)) sum++;
		sum += leafnum(tree->lchild);
		sum += leafnum(tree->rchild);
	}

	return sum;
}

// 清空树
status clear(btnode *tree)
{
	if (tree) destory(tree);
	tree = NULL;
	return OK;
}

// 判空
status empty(btnode *tree)
{
	if (tree) return OK;
	return ERROR;
}

// 初始化双亲指针
// 将左孩子指向自己
// 将右孩子指向自己
// 对左孩子，右孩子也如此
status init_parent(btnode *tree)
{
	static int f = 1;
	
	// 根结点没有双亲, 只能执行一次
	if (tree)
	{
		if (f) {f=0; tree->parent = NULL;}
		if (tree->lchild)
			tree->lchild->parent = tree;
		if (tree->rchild)
			tree->rchild->parent = tree;
		init_parent(tree->lchild);
		init_parent(tree->rchild);
	}
	return OK;
}

// 返回双亲地址
btnode *parent(btnode *tree, elemtype *data)
{
	btnode *addr;

	addr = find(tree, data);
	if (!addr) return NULL;
	return addr->parent;
}

// 返回根结点地址
btnode *root(btnode *tree)
{
	if (!tree) return NULL;
	while (tree->parent)
		tree = tree->parent;

	return tree;
}


// -----------顺序栈函数-------------
// 本来可以用上面的 rear front
// 但是为了区分，重新用两个变量
static int top;
static int base;
#define STACK_SIZE 150

status push(btnode *s[STACK_SIZE], btnode *data)
{
	if (top == base + STACK_SIZE) return ERROR;
	s[top++] = data;
	return OK;
}

btnode *pop(btnode *s[STACK_SIZE])
{
	if (top == base) return NULL;
	return s[--top];
}

btnode *gettop(btnode *s[STACK_SIZE])
{
	if (top == base) return NULL;
	return s[top - 1];
}

// 先序遍历 (非递归，栈)
// 执行visit操作，后进栈
// 直到没得进栈了，就出栈，直到可以向右走
// 直到栈为空，结束遍历
status first_tra_s(btnode *tree, status (*visit)(btnode *node))
{
	btnode *stack[STACK_SIZE];
	btnode *temp;

	if (!tree) return ERROR;
	top = base = 0;
	visit(tree);
	push(stack, tree);
	while (top != base)
	{
		temp = gettop(stack);  // 从栈里拿出一个
		if (!temp) break;      // 安全性
		while (temp->lchild)   // 一直向左走
		{
			visit(temp->lchild);
			push(stack, temp->lchild);
			temp = temp->lchild;
		}
		while (1)
		{
			temp = pop(stack);
			if (temp->rchild) break; // 直到 temp 有了右孩子
			if (top == base)         // 或者栈空了
			{
				temp = NULL;
				break;
			}
		}
		if (temp)
		{
			visit(temp->rchild);   // 向右走一步
			push(stack, temp->rchild);
		}
	}
	return OK;
}

// 中序遍历 (非递归， 栈)
// 一直向左走，出来一个直到可以向右走
status mid_tra_s(btnode *tree, status (*visit)(btnode *node))
{
	btnode *s[STACK_SIZE];
	btnode *t;

	top = base = 0;
	if (!tree) return 0;
	push(s, tree);		// 根结点入栈
	while (top != base)
	{
		t = gettop(s);
		if (!t) break;
		while (t->lchild)	// 一直向左走
		{
			push(s, t->lchild);
			t = t->lchild;
		}
		while (1)		// 找到一个可以向右走的结点
		{
			t = pop(s);
			if (t) visit(t);
			if (t->rchild) break;
			if (top == base) break;
		}
		if (t) push(s, t->rchild);	// 向右走
	}

	return OK;
}

// 后序遍历 (非递归， 栈)
status last_tra_s(btnode *tree, status (*visit)(btnode *node))
{
	btnode *s[STACK_SIZE];
	btnode *t;
	btnode *right;
	// 记录 t 是从哪个右孩子回来的
	// 如果 t->rchild == right
	// 说明是从右回到根的过程

	top = base = 0;
	if (!tree) return 0;
	push(s, tree);		// 根结点入栈
	while (top != base)
	{
		t = gettop(s);
		if (!t) break;
		while (t->lchild)	// 一直向左走
		{
			push(s, t->lchild);
			t = t->lchild;
		}
		while (1)	// 一直找到一个可以向右走的
		{
			right = t;
			t = gettop(s);
			// 没有右孩子或从右孩子回来时才访问
			if (!t->rchild || t->rchild == right)
			{
				visit(t);
				pop(s);
			}
			else break;
			if (top == base) break;
		}
		if (t && t->rchild != right)
			push(s, t->rchild);
	}
	return OK;
}

