// 二叉树的链式储存

typedef int status;
typedef char elemtype;

#define OK 1
#define ERROR 0
#define SIZE 80

// 二叉树结点结构
typedef struct btnode
{
	elemtype data;
	btnode *lchild;   // 左孩子
	btnode *rchild;   // 右孩子
	btnode *parent;   // 双亲
}btnode;

// ----------二叉树基本操作函数-----------
status create(btnode **tree);		// 输入树
char   *create2(btnode **tree, char *str);	// 输入树,根据str来建树
status destory(btnode *tree);		// 销毁树
status clear(btnode *tree);			// 清空树
status empty(btnode *tree);			// 判空
int    depth(btnode *tree);			// 求树的深度
int    leafnum(btnode *tree);		// 求叶子树
int    nodenum(btnode *tree);		// 返回结点个数

// 查找有没有key的结点, 返回地址
btnode *find(btnode *tree, elemtype *key);
// 更新key结点的值，data
status update(btnode *tree, elemtype *key, elemtype *data);
// 插入一个孩子结点 rl表示 0左 1右
status insert(btnode *tree, elemtype *key, elemtype *data, char rl);
// 删除一个孩子结点 或 树枝
status remove(btnode *tree, elemtype *key, char rl);
// 返回双亲结点地址
btnode *parent(btnode *tree, elemtype *data);
// 返回根结点地址
btnode *root(btnode *tree);
// 初始化双亲指针
status init_parent(btnode *tree);

// 先序遍历 (递归), 执行 visit 操作
status first_tra(btnode *tree, status (*visit)(btnode *node));
// 中序遍历 (递归)
status mid_tra(btnode *tree, status (*visit)(btnode *node));
// 后序遍历 (递归)
status last_tra(btnode *tree, status (*visit)(btnode *node));
// 层次遍历 (队列广搜)
status wild_tra(btnode *tree, status (*visit)(btnode *node));
// 先序遍历 (非递归，栈)
status first_tra_s(btnode *tree, status (*visit)(btnode *node));
// 中序遍历 (非递归， 栈)
status mid_tra_s(btnode *tree, status (*visit)(btnode *node));
// 后序遍历 (非递归， 栈)
status last_tra_s(btnode *tree, status (*visit)(btnode *node));
