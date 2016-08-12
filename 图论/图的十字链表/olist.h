/* -------------------------------------------
// 图的十字链表储存及相关操作
//
//
// 十字链表部分关系如图: 
// [0][A] -> [0][1] -> [0][3] -> [0][4]
// [1][B] -> [1][0]
// [2][C]
// [3][D] -> [3][2]
// [4][E] => [4][0] -> [4][1]
//
// 十字链表适合有向图的存储, 只测试有向图
// 十字链表下的广搜深搜和邻接表相同, 不再写
// 邻接多重表的操作和十字链表基本相同, 不再写
-------------------------------------------- */

typedef char elemtype;
typedef char infotype;
typedef int status;
#define ERROR 0
#define OK 1
#define NUM 30		// 最大结点数
#define NO -1		// 没找到


// 十字链表结点结构  |from|to|fromarc|toarc|
typedef struct vnode
{
	int from;			// 到我的顶点
	int to;				// 可到的顶点
	vnode *fromarc;		// 头相同的邻接点
	vnode *toarc;		// 尾相同的邻接点
	infotype *info;		// 附加信息
}vnode;

// 十字链表头结构  |顶点|fromarc|toarc|
typedef struct 
{
	elemtype data;		// 顶点名称
	vnode *fromarc;		// 第一条出边
	vnode *toarc;		// 第一条入边
}olist;

// 十字链表
typedef struct 
{
	olist vex[NUM];		// 头结构
	int vexnum;			// 顶点数
	int arcnum;			// 边数
}olgraph;


// ------------------十字链表存储, 图的相关操作----------------------

status create(olgraph *graph, elemtype *vexs, int n);
status destory(olgraph *graph);			// 销毁图
void input(olgraph *graph);				// 批量输入
void show(olgraph *graph);				// 输出十字链表
int find_vex(olgraph *graph, elemtype *data);		// 查找结点地址
status insert_arc(olgraph *graph, elemtype *st, elemtype *end);
status remove_arc(olgraph *graph, elemtype *st, elemtype *end);
int first_vex(olgraph *graph, elemtype *name);
int next_vex(olgraph *graph, elemtype *name, elemtype *pos);
