/* --------------------------------------
// 二叉树的复制
// 建立一颗 A 树, 将 A 树复制到 B 树
// 几种遍历方式, 均可以访问到全部结点
// 这里采用先序遍历来进行复制
//
// 另外包含三种非递归遍历的简洁代码
----------------------------------------*/

#include <stdio.h>
#include <malloc.h>

typedef int status;
typedef char elemtype;

// 二叉树存储结构
typedef struct btnode
{
	elemtype data;
	btnode *lchild;
	btnode *rchild;
}btnode;

// 根据字符串建树
char *create(btnode **t, char *str)
{
	if (*str == '#') (*t) = NULL;
	else if (*str != '\0')
	{
		(*t) = (btnode *)malloc(sizeof(btnode));
		if (!(*t)) return NULL;
		(*t)->data = *str;
		(*t)->lchild = NULL;
		(*t)->rchild = NULL;
		// 递归建树
		str = create(&((*t)->lchild), str + 1);
		str = create(&((*t)->rchild), str + 1);
	}
	return str;
}

// 先序遍历 (非递归)
void first_tra_s(btnode *t)
{
	btnode *stack[100];
	int top = 0;

	while (t || top)
	{
		while (t)
		{
			stack[top++] = t;
			printf("%c, ", t->data);
			t = t->lchild;
		}
		// 试探回去一步
		if (top) t = stack[--top]->rchild;
	}
}

// 中序遍历 (非递归)
void mid_tra_s(btnode *t)
{
	btnode *stack[100];
	int top = 0;

	while (t || top)
	{
		// 向左走到底
		while (t)
		{
			stack[top++] = t;
			t = t->lchild;
		}
		// 此时已走到底, 出栈一个访问
		if (top != 0)
		{
			t = stack[--top];
			printf("%c, ", t->data);
		}
		// 如果有右孩子, 向右跨一步
		// 没有就要及时将 t 置空, 防止被上面的while走回去
		if (t->rchild) t = t->rchild;
		else t = NULL;
	}
}

// 后序遍历 (非递归)
void last_tra_s(btnode *t)
{
	btnode *stack[100];
	int top = 0;
	btnode *right;		// 记录前驱结点

	do	// 先执行, 直到栈为空
	{
		while (t)	// 走到最底
		{
			stack[top++] = t;
			t = t->lchild;
		}
		while (top)
		{
			right = t;
			t = stack[top - 1];		// 取栈顶
			if (t->rchild == NULL || t->rchild == right)
			{
				printf("%c, ", t->data);
				top--;		// 确定以后才能出栈
			}
			else	// 发现可以向右走的了
			{
				t = t->rchild;
				break;
			}
		}
	} while (top);
}

// 先序遍历 t 树, 复制到 n 树
// 类似创建二叉树的算法
void copytree(btnode *t, btnode **n)
{
	if (!t) (*n) = NULL;
	else 
	{
		(*n) = (btnode *)malloc(sizeof(btnode));
		if (!(*n)) return ;
		(*n)->data = t->data;

		// 直接在 n 的左右孩子上建立新结点
		copytree(t->lchild, &((*n)->lchild));
		copytree(t->rchild, &((*n)->rchild));
	}
}

int main(void)
{
	btnode *tree;
	btnode *tree2;
	char str[100];

	// ABCH###D##EFJ###G#I##
	printf("请输入二叉树: ");
	scanf("%s", str);
	create(&tree, str);

	printf("tree中序: ");
	mid_tra_s(tree);
	printf("\n");

	copytree(tree, &tree2);   // 复制二叉树到tree2
	printf("tree2 :\n");
	printf("先序遍历: ");
	first_tra_s(tree2);
	printf("\n");
	printf("中序遍历: ");
	mid_tra_s(tree2);
	printf("\n");
	printf("后序遍历: ");
	last_tra_s(tree2);
	printf("\n");

	return 0;
}