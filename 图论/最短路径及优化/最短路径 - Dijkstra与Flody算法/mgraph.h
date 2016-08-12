// 图的邻接矩阵的ADT

#define OK 1
#define ERROR 0
#define INF 0x5FFFFFF	// 无限大
#define NO -1			// 找不到
#define MAX_NUM 30		// 最大顶点数
#define SIZE 100		// 队列，栈大小

// 无限大说明: 无限大不能定太大, 因为如要进行 INF + INF 容易发生溢出

typedef int elemtype;
typedef int status;
typedef int vrtype;		// 边的权值
typedef char nametype;	// 顶点名称
typedef char intype;	// 附加信息

// 有向网, 无向网
typedef enum {DN, UDN} graphkind;

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
status input(mgraph *graph);						// 批量输入边
int find_vex(mgraph *graph, nametype *data);		// 寻找 data 的位序
status insertarc(mgraph *graph, nametype *a, nametype *b, int quan);  // 插入边
