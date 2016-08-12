// 图的邻接矩阵的ADT

#define OK 1
#define ERROR 0
#define CHARINF '-'		// 无限大的表示
#define INF 0x5FFFFFFF	// 无限大
#define NOT_FOUND -1	// 找不到
#define MAX_NUM 50		// 最大顶点数
#define SIZE 100		// 队列，栈大小

typedef int elemtype;
typedef int status;
typedef int vrtype;		// 边的权值, 无向图可用bool类型
typedef char nametype;	// 顶点名称
typedef char intype;	// 附加信息

extern unsigned short int used[MAX_NUM];	// 说明外部有个 used 数组

// D: direction有向  G: graph图  N: net网
typedef enum {DG, UDG, DN, UDN} graphkind;

// 单个边结构
typedef struct arccell
{
	vrtype adj;			// 边的权值
	intype *info;		// 附加信息
}arccell;

// 图的邻接矩阵储存
typedef struct mgraph
{
	nametype vexs[MAX_NUM];				// 顶点集合, 如v1,v2,v3
	arccell  arcs[MAX_NUM][MAX_NUM];	// 边的集合
	int vexnum;							// 顶点数
	int arcnum;							// 边数
	graphkind kind;						// 图的种类
}mgraph;
// arcs 可以根据顶点数动态申请

// 邻接矩阵的相关操作
status create(mgraph *graph, int kind, int n, nametype *vex);
status create_G(mgraph *graph, graphkind kind, int n, nametype *vex);
status create_N(mgraph *graph, graphkind kind, int n, nametype *vex);
status input(mgraph *graph);				  // 批量输入边
status show(mgraph *graph);					  // 输出邻接矩阵
int find_vex(mgraph *graph, nametype *data);  // 寻找 data 的位序
status destory(mgraph *graph);				  // 清空图
status getvex(mgraph *graph, int pos, nametype *back);     // 取得第 i 个点的名称
status putvex(mgraph *graph, nametype *old, nametype *data);  // 更新顶点名称
int firstadjvex(mgraph *graph, nametype *key);				  // 返回第一个邻接点
int nextadjvex(mgraph *graph, nametype *old, nametype *key);  // 返回下一个邻接点
status insertvex(mgraph *graph, nametype *data);			  // 插入新结点
status deletevex(mgraph *graph, nametype *data);			  // 删除结点
status insertarc(mgraph *graph, nametype *a, nametype *b, int quan);	// 插入边
status deletearc(mgraph *graph, nametype *a, nametype *b);		// 删除边
status deleteallarc(mgraph *graph, nametype *data);				// 删除data的所有边
status graph_DFS(mgraph *graph, status (*visit)(nametype *data));     // 深度搜索，递归
status graph_DFS_s(mgraph *graph, status (*visit)(nametype *data));   // 深度搜索, 栈
status DFS(mgraph *graph, int addr, status (*visit)(nametype *data)); // 搜索单个连通分支
status graph_BFS(mgraph *graph, status (*visit)(nametype *data));     // 广度搜索，队列
