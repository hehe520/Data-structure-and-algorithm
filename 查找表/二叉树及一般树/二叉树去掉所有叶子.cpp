#include <stdio.h>
#include <malloc.h>

#define OK 1
#define ERROR 0
typedef int status;

typedef struct 
{
	char key;
	char *info;
}elemtype;

// 二叉链表
typedef struct btnode
{
	elemtype data;
	btnode *lchild;
	btnode *rchild;
}btnode;

// 函数功能专一性，所以不做字符检查，
// 字符检查交给其他函数
char* create(btnode **tree, char *str)
{
	if (str[0] == '\0') return ERROR;
	if (*str == '#') *tree = NULL;
	else 
	{
		// 创建新结点
		*tree = (btnode *)malloc(sizeof(btnode));
		if (!(*tree)) return ERROR;
		(*tree)->data.key = *str;
		(*tree)->lchild = NULL;
		(*tree)->rchild = NULL;
	
		// 归纳
		str = create(&((*tree)->lchild), str + 1);
		str = create(&((*tree)->rchild), str + 1);
	}
	return str; 
}

// 先序递归遍历
status first_tra(btnode *tree)
{
	if (tree)
	{
		printf("%c, ", tree->data.key);
		first_tra(tree->lchild);
		first_tra(tree->rchild);
		return OK;
	}
	return ERROR;
}

int lenth(btnode *tree)
{
	int left, right;

	// 空树的深度是 0
	if (!tree) return 0;
	else 
	{
		left = lenth(tree->lchild);
		right = lenth(tree->rchild);
		left = left > right ? left : right;
	}
	return left + 1;
}

// 去掉所有树叶，
status cuttree(btnode **tree, btnode *parent)
{
	if (!(*tree)) return ERROR;
	else
	{
		// 如果是叶子就释放掉
		if ((*tree)->lchild == NULL 
			&& (*tree)->rchild == NULL)
		{
			free(*tree);
			if (!parent) *tree = NULL;
			else if (parent->lchild == *tree)
				parent->lchild = NULL;
			else if (parent->rchild == *tree)
				parent->rchild = NULL;
		}
		else   // 不是叶子就继续遍历孩子
		{
			cuttree(&((*tree)->lchild), *tree);
			cuttree(&((*tree)->rchild), *tree);
		}
	}
	return OK;
}

int main(void)
{
	btnode *tree;
	char str[100] = {0};

	// ABCH###D##EFJ###G#I##
	printf("先序建树：");
	scanf("%s", str);
	create(&tree, str);
	printf("先序遍历：");
	first_tra(tree);
	printf("\n树的深度：%d\n", lenth(tree));
	
	while (1)   // 不断去掉叶子
	{
		getchar();
		cuttree(&tree, NULL);
		printf("\n去掉叶子后：");
		first_tra(tree);
		printf("\n树的深度：%d\n", lenth(tree));
	}

	return 0;
}
