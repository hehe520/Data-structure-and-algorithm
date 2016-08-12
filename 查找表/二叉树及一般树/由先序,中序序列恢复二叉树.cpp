// 根据先序序列 和 中序序列 恢复二叉树
// 可用长度为 0 , 或者先序为 \0 返回空
// 否则建立结点, 分割区间
// 分割先序序列,如下图
// pos = 3   即:'A'
// 先序 A BCD EFG
// 中序 CBD A FEG
// 先序左区间BCD, 右区间EFG, 先序即 f+1, f+pos+1, 长度 pos
// 中序左区间CBD, 右区间FEG, 中序即 m,   m+pos+1, 长度 len-pos
// 左孩子用左区间,,右孩子用右区间,,

#include <stdio.h>
#include <string.h>
#include <malloc.h>

typedef char elemtype;

// 二叉树结点
typedef struct btnode
{
	elemtype data;
	btnode *lchild;
	btnode *rchild;
}btnode;


// 递归建树部分
// 返回建好的树根, 先序序列, 中序序列, 可用长度
btnode *createtree(elemtype *f, elemtype *m, int len)
{
	btnode *n;
	int i, pos;		// 记录根位置

	if (len <= 0 || *f == '\0') return NULL;  // 先序建完了
	
	// 基本项: 建立一个先序结点
	n = (btnode *)malloc(sizeof(btnode));
	if (!n) return NULL;
	n->data = *f;

	// 在中序 [0..len) 找*f
	for (i=0; i<len; i++)
		if (m[i] == *f) break;
	if (i == len) return NULL;	// i = len就是没找到,序列错误.
	else pos = i;

	// 分割先序序列, 方法在上面
	n->lchild = createtree(f + 1, m, pos);
	n->rchild = createtree(f + pos+1, m +pos+1, len-pos);

	return n;
}

// 根据先序,中序序列,恢复二叉树, 返回根节点地址
btnode *create(elemtype *f, elemtype *m)
{
	int len1, len2;

	// 先进行合法检查
	if (!(f && m)) return NULL;
	len1 = strlen(f);
	len2 = strlen(m);
	if (len1 != len2) return NULL;
	
	// 进入下一层函数,递归建树
	return createtree(f, m, len1);
}

// 先序遍历
void first_tra(btnode *tree)
{
	if (tree)
	{
		printf("%c, ", tree->data);
		first_tra(tree->lchild);
		first_tra(tree->rchild);
	}
}

int depth(btnode *tree)
{
	int a, b;
	
	if (!tree) return 0;
	a = depth(tree->lchild);
	b = depth(tree->rchild);
	return (a > b ? a : b) + 1;
}

// 释放
void destory(btnode *tree)
{
	if (tree)
	{
		destory(tree->lchild);
		destory(tree->rchild);
		free(tree);
	}
}

int main(void)
{
	char first[70];		// 先序
	char mid[70];		// 中序
	btnode *head;		// 头结点

	// 测试数据在上面
	printf("请输入先序序列: ");
	scanf("%s", first);
	printf("请输入中序序列: ");
	scanf("%s", mid);

	// 恢复二叉树
	head = create(first, mid);
	
	printf("\n先序遍历: ");
	first_tra(head);
	printf("\n树的深度: %d\n", depth(head));

	destory(head);
	return 0;
}
