
// 编译环境 VC++6.0, 用 C++ 编译器编译
// 本代码包含四个实验: 
// 均值滤波, 中值滤波, 图像锐化, 边缘检测

#include <stdio.h>
#include <string.h>
#include <malloc.h>

#define ABS(a) ((a) > 0 ? (a) : (-a))
typedef unsigned char UC;
typedef unsigned long int ULI;
typedef struct		// ppm 图像
{
	char type;		// P 标志
	char flag;		// 几号
	int  width;		// 宽
	int  height;	// 长
	int  maxv;		// 最大量化值
	int  byte;		// 每个像素占用字节数
	UC   *data;		// 图像数据
}IMG;

// 四个均值滤波模板
int board1[9] = {1,2,1, 2,4,2, 1,2,1};
int board2[9];		// 全 1
int board3[25];		// 全 1
int board4[49];		// 全 1

// 两个中值滤波模版, 1 考虑, 0 不考虑
int middle1[25] = {0,0,1,0,0, 0,0,1,0,0, 1,1,1,1,1, 0,0,1,0,0, 0,0,1,0,0};
int middle2[25] = {0,0,1,0,0, 0,1,1,1,0, 1,1,1,1,1, 0,1,1,1,0, 0,0,1,0,0};

// 两个 Laplace 算子
int laplace1[9] = {0,-1,0, -1,5,-1, 0,-1,0};
int laplace2[9] = {-1,-1,-1, -1,9,-1, -1,-1,-1};

// 两个 Sobel 算子, prewitt 算子
int sobel_x[9] = {1,2,1, 0,0,0, -1,-2,-1};
int sobel_y[9] = {-1,0,1, -2,0,2, -1,0,1};
int prewitt_x[9] = {1,1,1, 0,0,0, -1,-1,-1};
int prewitt_y[9] = {-1,0,1, -1,0,1, -1,0,1};

// 函数声明
int open_img(IMG *ppm, char *path);		// 打开
int close_img(IMG *ppm);				// 关闭
int save_img(IMG *ppm, char *path);		// 保存
int copy_img(IMG *d, IMG const *s);		// 复制


// 滤波函数，ppm是原图，board是模板, h 长, w 宽, s 总和, result 是结果图
// 支持灰度图, 彩色图, 动态适应模版大小
void filters(IMG *ppm, int *board, int h, int w, int s, IMG *result)
{
	int i, j, ii, jj;
	int is = h / 2;					// 处理范围 [is, ie)
	int ie = ppm->height - is;
	int js = (w / 2) * ppm->byte;	// [js, je)
	int je = ppm->width * ppm->byte - js;
	int hh = h / 2;					// 长度范围 [-hh, hh]
	int ww = (w / 2) * ppm->byte;	// 宽度 [-ww, ww]
	int k = ppm->byte * ppm->width;		// 一行 k 个像素
	int cur;
	long int sum, tmp;
	UC *temp = (UC *)malloc(ppm->height * ppm->width * ppm->byte * sizeof(UC));
	// temp 卷积后的图像临时空间

	for (i=is; i<ie; i++)	// 卷积
	{
		for (j=js; j<je; j++)
		{
			// 原图从 [-inc, inc] 增量 byte 个
			// 彩色图像有三个分量, 宽是原来的三倍, 长不变
			cur = sum = 0;
			for (ii=-hh; ii<=hh; ii++)				// 纵向自增一个
				for (jj=-ww; jj<=ww; jj+=ppm->byte)		// 横向彩色图自增 3 个
					sum += ppm->data[(i + ii) * k + (j + jj)] * board[cur++];

			// 小于 0 设成取绝对值, 超过 maxv 设成 maxv, 为了边缘检测设计
			tmp = (long int)((double)sum / s + 0.5);
			tmp = ABS(tmp);
			if (tmp > (long int)ppm->maxv) tmp = (long int)ppm->maxv;
			temp[i * k + j] = (UC)tmp;
		}
	}
	cur = 0;
	for (i=is; i<ie; i++)	// 覆盖回原图像中
		for (j=js; j<je; j++)
			result->data[cur++] = temp[i * k + j];
	free(temp);
}

// 滤波函数, 先把原图按照模版大小拓宽, 再进行滤波, 以便处理到边缘
void my_filters(IMG *ppm, int *board, int h, int w, int s=1)
{
	IMG new_ppm;
	int new_h = h / 2;
	int new_w = (w / 2) * ppm->byte;
	int i, j, k;		// k 是每行的像素个数

	new_ppm = *ppm;
	new_ppm.height += (2 * (h / 2));		// 拓宽
	new_ppm.width += (2 * (w / 2));
	new_ppm.data = (UC *)malloc(new_ppm.height * new_ppm.width
		* new_ppm.byte * sizeof(UC));

	// 拓宽后在上下左右填充 0
	k = new_ppm.width * new_ppm.byte;
	for (j=0; j<k; j++)
	{
		for (i=0; i<new_h; i++)		
			new_ppm.data[i * k + j] = 0;	// 上边填充 0
		for (i=new_ppm.height-new_h; i<new_ppm.height; i++)
			new_ppm.data[i * k + j] = 0;	// 下边填充 0
	}
	for (i=0; i<new_ppm.height; i++)
	{
		for (j=0; j<new_w; j++)	
			new_ppm.data[i * k + j] = 0;	// 左边填充 0
		for (j=k-new_w; j<k; j++)
			new_ppm.data[i * k + j] = 0;	// 右边填充 0
	}

	// 把原图数据复制进来
	int is = h / 2;				// 处理范围 [is, ie)
	int ie = new_ppm.height - is;
	int js = (w / 2) * new_ppm.byte;	// [js, je)
	int je = new_ppm.width * new_ppm.byte - js;
	int cur = 0;
	for (i=is; i<ie; i++)
		for (j=js; j<je; j++)
			new_ppm.data[i * k + j] = ppm->data[cur++];

	// 然后进行滤波函数
	filters(&new_ppm, board, h, w, s, ppm);
	free(new_ppm.data);		// 别忘记释放内存
}

// 中值滤波, ppm 图像, board 模版, h 高, w 宽
// 支持灰度图, 彩色图, 动态适应模版大小
void median(IMG *ppm, int *board, int h, int w)
{
	int i, j, ii, jj;
	int is = h / 2;					// 处理范围 [is, ie)
	int ie = ppm->height - is;
	int js = (w / 2) * ppm->byte;	// [js, je)
	int je = ppm->width * ppm->byte - js;
	int hh = h / 2;					// 长度范围 [-hh, hh]
	int ww = (w / 2) * ppm->byte;	// 宽度 [-ww, ww]
	int k = ppm->byte * ppm->width;	// 一行 k 个像素
	int cur, sum;		// 1 的个数
	UC arr[100];		// 排序数组
	int index = 0;
	int top = 0;

	// temp 是临时空间, map 是计数排序的辅组空间
	UC *temp = (UC *)malloc(ppm->height * ppm->width * ppm->byte * sizeof(UC));
	int *map = (int *)malloc((ppm->maxv + 2) * sizeof(int));
	for (ii=0; ii<=ppm->maxv; ii++) map[ii] = 0;
	for (i=0, sum=0; i<w * h; i++) sum += board[i];		// 选中的像素个数
	for (i=is; i<ie; i++)
	{
		for (j=js; j<je; j++)		// 解释见上
		{
			top = index = cur = sum = 0;
			for (ii=-hh; ii<=hh; ii++)
				for (jj=-ww; jj<=ww; jj+=ppm->byte, cur++)
					if (board[cur])
						arr[index++] = ppm->data[(i + ii) * k + (j + jj)];

			// 计数排序快 O(n), 像素值一般是 0 - 255, 或者有更大的, 
			for (ii=0; ii<index; ii++) map[arr[ii]]++;	// 计数
			for (ii=0; ii<=ppm->maxv; ii++) 
				if (map[ii] != 0)
				{
					for (jj=0; jj<map[ii]; jj++) arr[top++] = ii;
					map[ii] = 0;		// 顺便清空
				}
			temp[i * k + j] = arr[index / 2];	// 取中值
		}
	}
	for (i=is; i<ie; i++)	// 覆盖回原图像中
		for (j=js; j<je; j++)
			ppm->data[i * k + j] = temp[i * k + j];
	free(temp);
	free(map);
}


// 边缘检测, 参数: 原图, 横向模版, 纵向模版, 模版高, 宽, 位移, 边缘阀值
void side_detect(IMG *ppm, int *board_x, int *board_y, 
			   int h, int w, int s, double c)
{
	int i, j, cur, tmp;
	IMG side_x;
	IMG side_y;
	
	memset(&side_x, 0, sizeof(IMG));
	memset(&side_y, 0, sizeof(IMG));
	copy_img(&side_x, ppm);
	copy_img(&side_y, ppm);

	// 分别滤波
	my_filters(&side_x, board_x, h, w, s);
	my_filters(&side_y, board_y, h, w, s);

	// 合成纵横两张图
	int k = ppm->width * ppm->byte;
	for (i=0; i<ppm->height; i++)
		for (j=0; j<k; j++)
		{
			cur = i * k + j;
			// 合成函数 g(x,y) = C * (|gx(x,y)| + |gy(x,y)|);
			tmp = (int)(c * (ABS(side_x.data[cur]) + ABS(side_y.data[cur])));
			if (tmp > ppm->maxv) tmp = ppm->maxv;
			if (tmp < 0) tmp = 0;
			ppm->data[cur] = (UC)tmp;
		}
	close_img(&side_x);
	close_img(&side_y);
}

int main(void)
{
	IMG ppm;			// 原图
	IMG tmp;			// 结果图
	char path[512];		// 路径
	int i, number;

	// 支持灰度图和彩色图
	printf("输入图片路径: 或把图片直接拖到本窗口上, 然后会弹出菜单\n");
	scanf("%s", path);
	memset(&ppm, 0, sizeof(ppm));	// 初始化数据
	memset(&tmp, 0, sizeof(tmp));
	for (i=0; i<9; i++) board2[i] = 1;
	for (i=0; i<25; i++) board3[i] = 1;
	for (i=0; i<49; i++) board4[i] = 1;

	if (open_img(&ppm, path))
	{
		printf("\n处理图像: \n\n   1. 均值滤波\n\n   2. 中值滤波\n\n");
		printf("   3. 图像锐化\n\n   4. 边缘检测\n\n---请输入操作: ");
		scanf("%d", &number);
		printf("处理中,,,");
		if (number == 1)		// 均值滤波
		{
			copy_img(&tmp, &ppm);		// 不破坏原图，重新 copy 一份
			my_filters(&tmp, board1, 3, 3, 16);	// 滤波模板1
			save_img(&tmp, "均值滤波1.ppm");

			copy_img(&tmp, &ppm);
			my_filters(&tmp, board2, 3, 3, 9);	
			save_img(&tmp, "均值滤波2.ppm");

			copy_img(&tmp, &ppm);
			my_filters(&tmp, board3, 5, 5, 25);
			save_img(&tmp, "均值滤波3.ppm");

			copy_img(&tmp, &ppm);
			my_filters(&tmp, board4, 7, 7, 49);
			save_img(&tmp, "均值滤波4.ppm");
		}
		else if (number == 2)	// 中值滤波
		{
			copy_img(&tmp, &ppm);
			median(&tmp, middle1, 5, 5);
			save_img(&tmp, "中值滤波1.ppm");

			copy_img(&tmp, &ppm);
			median(&tmp, middle2, 5, 5);
			save_img(&tmp, "中值滤波2.ppm");
		}
		else if (number == 3)	// 图像锐化
		{
			copy_img(&tmp, &ppm);
			my_filters(&tmp, laplace1, 3, 3, 1);
			save_img(&tmp, "laplace锐化1.ppm");

			copy_img(&tmp, &ppm);
			my_filters(&tmp, laplace2, 3, 3, 1);
			save_img(&tmp, "laplace锐化2.ppm");
		}
		else if (number == 4)	// 边缘检测
		{
			copy_img(&tmp, &ppm);
			side_detect(&tmp, sobel_x, sobel_y, 3, 3, 1, 0.7);
			save_img(&tmp, "sobel边缘检测.ppm");

			copy_img(&tmp, &ppm);
			side_detect(&tmp, prewitt_x, prewitt_y, 3, 3, 1, 0.7);
			save_img(&tmp, "prewitt边缘检测.ppm");
		}
		close_img(&tmp);	// 释放内存
		close_img(&ppm);
		if (number >=1 && number <= 4) printf("\n完成!\n");
		else printf("\n输入错误!\n");
	}
	else printf("只能打开灰度和彩色ppm图\n");

	return 0;
}

// ------------------------------------------------------------
// ---------------- 以下代码不是本实验重点 --------------------
// ------------------------------------------------------------

// 读取一张图片, 只能读取灰度图和彩色图
int open_img(IMG *ppm, char *path)
{
	FILE *fp = fopen(path, "rb");
	int ret = 1;	// 返回值
	int w, h;
	int i, f;

	if (fp != NULL)
	{
		ppm->type = fgetc(fp);
		ppm->flag = fgetc(fp);
		fgetc(fp);		// 多余空格
		fscanf(fp, "%d", &(ppm->width));
		fgetc(fp);
		fscanf(fp, "%d", &(ppm->height));
		fgetc(fp);
		fscanf(fp, "%d", &(ppm->maxv));
		fgetc(fp);

		// 分配内存空间
		w = ppm->width;
		h = ppm->height;
		if (ppm->flag == '5')
		{
			ppm->data = (UC *)malloc(w * h * sizeof(UC));
			ppm->byte = 1;
		}
		else if (ppm->flag == '6')
		{
			ppm->data = (UC *)malloc(w * h * 3 * sizeof(UC));
			ppm->byte = 3;
		}
		else ret = 0;

		// 读取数据
		if (ret != 0 && ppm->data != NULL)
		{
			// 读取数据, fread() 一次只能读入 64KB 的数据, 这里分段读
			for (i=0, f=1; i<h && f!=0; i++)
			{
				f = fread(ppm->data + i * w * ppm->byte, 1, w * ppm->byte, fp);
			}
			if (f == 0)		// 一部分数据读取失败
			{
				free(ppm->data);
				ret = 0;
			}
		}
		else ret = 0;
		fclose(fp);
	}
	else ret = 0;

	return ret;
}

// 写入一张图片
int save_img(IMG *ppm, char *path)
{
	FILE *fp = fopen(path, "wb");
	int w = ppm->width;
	int h = ppm->height;
	int i, f;

	if (fp == NULL) return 0;

	// 写入 ppm 头
	fprintf(fp, "%c%c\n%d\n%d\n%d\n", 
		ppm->type, ppm->flag, w, h, ppm->maxv);

	// 写入图像数据
	for (i=0, f=1; i<h && f!=0; i++)
	{
		f = fwrite(ppm->data + i * w * ppm->byte, 1, w * ppm->byte, fp);
	}
	if (f == 0) return 0;	// 写入错误
	fclose(fp);

	return 1;
}

int copy_img(IMG *d, IMG const *s)		// 复制
{
	if (d == NULL || s == NULL) return 0;

	if (d->data != NULL) free(d->data);		// 释放原来的图像
	*d = *s;								// 复制成员
	d->data = (UC *)malloc(s->height * s->width * d->byte * sizeof(UC));
	int size = s->height * s->width * s->byte;

	for (int i=0; i<size; i++)
		d->data[i] = s->data[i];	// 复制图像数据

	return 0;
}

// 关闭一张图片
int close_img(IMG *ppm)
{
	if (ppm->data) free(ppm->data);
	memset(ppm, 0, sizeof(IMG));

	return 0;
}
