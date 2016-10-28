
// 本程序提供写入水印, 读取水印, 清除水印的功能
// 水印用 bitplane 方法嵌入, 详细设计见报告
// 编译环境 VC++6.0, C++编译

#include <stdio.h>
#include <string.h>
#include <malloc.h>

typedef unsigned char UC;
typedef unsigned long int ULI;
typedef struct		// ppm 图像结构体
{
	char type;		// P 标志
	char flag;		// 几号
	char byte;		// 每个像素占用字节数
	int  width;		// 宽
	int  height;	// 长
	int  maxv;		// 最大量化值
	UC   *data;		// 图像数据
}IMG;

const char *const WATER = "+-:";	// 水印起始标志

int open_img(IMG *ppm, char *path);						// 打开 P4, P5, P6 皆可
int save_img(IMG *ppm, char *path);						// 保存
int copy_img(IMG *d, IMG const *s);						// 复制
int close_img(IMG *ppm);								// 关闭
int extract_bit(IMG *d, IMG const *s, int bit);			// 提取比特
int update_bit(IMG *d, IMG const *s, int bit);			// 合成比特
int extract_rgb(IMG *ppm, IMG *r, IMG *g, IMG *b);		// 提取 RGB 三个分量
int combine_rgb(IMG *ppm, IMG *r, IMG *g, IMG *b);		// 合成 RGB 三个分量
int read_watermaking(IMG *ppm, char *data);				// 读取水印
int write_watermaking(IMG *ppm, char *data, char c);	// 写入水印
int clear_watermaking(IMG *ppm);						// 清除水印
ULI calc_size(IMG *ppm);								// 计算能写入的字节数
int check(char *data, ULI size);						// 合法检查
int display_menu(IMG *ppm);								// 显示菜单


int main(void)
{
	IMG ppm;			// 图像
	char path[512];		// 路径

	printf("输入图片路径: 或把图片直接拖到本窗口上\n");
	gets(path);
	if (open_img(&ppm, path))
	{
		display_menu(&ppm);		// 显示菜单并处理
		close_img(&ppm);		// 记得关闭
	}
	else printf("打开错误!\n");

	return 0;
}

// ------------------------------------------------------------
// -------------------- 水印相关的函数 ------------------------
// ------------------------------------------------------------

// 读取水印, 读取到 data 中 
int read_watermaking(IMG *ppm, char *data)	
{
	IMG bit, r, g, b;
	int i, times, top;
	char *cur, *end;	// 查找水印的指针

	if (ppm == NULL || data == NULL) return 0;

	bit.data = r.data = g.data = b.data = NULL;
	if (ppm->flag == '5')
	{
		copy_img(&b, ppm);
	}
	else if (ppm->flag == '6')
	{
		extract_rgb(ppm, &r, &g, &b);	// 提取三个分量
		close_img(&r);
		close_img(&g);
	}
	else return 0;

	for (i=0; i<3; i++)		// 在 0-2 比特内寻找水印
	{
		extract_bit(&bit, &b, i);
		bit.data[bit.height * (bit.width / bit.byte) - 1] = '\0';
		times = 0;
		cur = (char *)bit.data;
		while (1)
		{
			// 防止随机的数据也能产生水印标志
			// while 循环里要找出 4 次水印标志才算找到水印
			// 随机数据正好产生 4 次水印标志的概率很小
			cur = strstr(cur, WATER);
			if (cur == NULL) break;
			else times++;
			if (times == 4) break;
		}
		if (times == 4) break;
	}
	close_img(&b);

	if (times != 4) 	// 没找到
	{
		close_img(&bit);
		return 0;
	}
	cur = (char *)bit.data;
	cur = strstr(cur, WATER);		// 找第一个标志
	end = strstr(cur + 1, WATER);	// 找下一个标志
	cur += strlen(WATER);			// 跳过起始标志
	top = 0;
	while (cur != end)
	{
		data[top++] = *cur++;
	}
	data[top] = '\0';
	close_img(&bit);

	return 1;
}

// 写入水印, 支持彩色图和灰度图
int write_watermaking(IMG *ppm, char *data, char c)		
{
	IMG bit;			// 第 c 比特
	ULI size;			// bit 大小
	ULI top = 0;		// bit 下标
	int index = 0;		// 水印下标
	int len = 0;		// 水印长度
	char *tmp = NULL;	// 水印信息

	c = c - '0' - 1;	// 提取第 c 比特信息
	if (c < 0 || c > 3) return 0;
	if (!(ppm->flag != '5' || ppm->flag != '6')) return 0;

	bit.data = NULL;	// 初始化 bit
	bit = *ppm;
	bit.flag = '4';
	bit.byte = 8;
	size = bit.width * bit.height / bit.byte;
	bit.data = (UC *)malloc(size * sizeof(UC));

	len = strlen(WATER) + strlen(data);		// 初始化水印 tmp
	tmp = (char *)malloc((len + 2) * sizeof(char));
	if (tmp == NULL) return 0;
	strcpy(tmp, WATER);
	strcat(tmp, data);

	while (top < size)		// 循环写入水印信息
	{
		bit.data[top++] = tmp[index];
		index = (index + 1) % len;
	}
	free(tmp);

	// 写回水印信息
	if (ppm->flag == '5')
	{
		update_bit(ppm, &bit, c);	// 写入到第 c 比特
	}
	else if (ppm->flag == '6')		// 彩色图
	{
		IMG r, g, b;

		r.data = g.data = b.data = NULL;
		extract_rgb(ppm, &r, &g, &b);	// 提取蓝色分量
		update_bit(&b, &bit, c);		// 写入到第 c 比特
		combine_rgb(ppm, &r, &g, &b);	// 写回蓝色分量

		close_img(&r);
		close_img(&g);
		close_img(&b);
	}
	close_img(&bit);
	
	return 1;
}

void clear_gray(IMG *ppm)	// 清除灰度图的水印
{
	IMG bit;
	char *cur;
	int len = strlen(WATER);
	int i, j;
	
	bit.data = NULL;
	for (i=0; i<3; i++)		// 在 0-2 比特内寻找水印
	{
		extract_bit(&bit, ppm, i);		// 提取比特
		bit.data[bit.height * (bit.width / bit.byte) - 1] = '\0';
		cur = (char *)bit.data;
		while (1)
		{
			cur = strstr(cur, WATER);
			if (cur == NULL) break;
			// 随机改一个其它符号
			for (j=0; j<len; j++) cur[j] = j + 1;
		}
		update_bit(ppm, &bit, i);		// 写回去掉水印的比特
	}
}

int clear_watermaking(IMG *ppm)		// 清除水印
{
	IMG r, g, b;

	if (ppm == NULL) return 0;

	r.data = g.data = b.data = NULL;
	if (ppm->flag == '5')
	{
		clear_gray(ppm);
	}
	else if (ppm->flag == '6')
	{
		extract_rgb(ppm, &r, &g, &b);	// 提取三个分量
		clear_gray(&b);
		combine_rgb(ppm, &r, &g, &b);
		close_img(&r);
		close_img(&g);
		close_img(&b);
	}
	else return 0;

	return 1;
}

ULI calc_size(IMG *ppm)		// 计算能写入的字节数
{
	// (总字节 - 标志长度 - '\0') / 4
	return ((ULI)(ppm->height * ppm->width / 8) - strlen(WATER) - 1) / 4;
}

// ------------------------ 主菜单 ----------------------------
int display_menu(IMG *ppm)
{
	char data[2048];	// 水印信息, 注意大小
	char c;

	fflush(stdin);
	printf("\n1.读取水印\t2.写入水印\t3.清除水印\t请输入: ");
	c = getchar();
	if (c == '1')			// 读取水印
	{
		if (read_watermaking(ppm, data))
			printf("\n水印信息: %s\n", data);
		else printf("\n没有水印!\n");
	}
	else if (c == '2')		// 写入水印
	{
		ULI size = calc_size(ppm);
		printf("\n可写入信息的字节数 %u byte\n", size);
		printf("请输入嵌入的信息: ");
		fflush(stdin);
		gets(data);
		if (check(data, size))
		{
			printf("请输入水印的强度: 1.低  2.中  3.高 (更耐久): ");
			c = getchar();
			if (write_watermaking(ppm, data, c)) 
			{
				printf("写入成功!\n");
				save_img(ppm, "写入水印后.ppm");
			}
			else printf("写入失败\n");
		}
		else printf("水印信息不能有 + 号\n");
	}
	else if (c == '3')		// 清除水印
	{
		clear_watermaking(ppm);
		save_img(ppm, "清除水印后.ppm");
		printf("清除成功!\n");
	}
	else printf("输入错误!\n");

	return 1;
}

// ------------------------------------------------------------
// ------------------ 图像操作的相关函数 ----------------------
// ------------------------------------------------------------

// d 是结果图像(二值), s 是灰度图, 提取第 bit 个比特
int extract_bit(IMG *d, IMG const *s, int bit)		// 提取比特
{
	UC b = 1;		// 提取位的模版
	UC tmp;
	int i, j, size;
	int top = 0;	// s 图像的下标

	if (d == NULL || s == NULL || s->data == NULL 
		|| bit < 0 || bit > 7) return 0;

	if (d->data) free(d->data);		// 释放原来的图像
	*d = *s;
	d->flag = '4';
	d->byte = 8;
	d->maxv = 1;
	size = d->height * (d->width / d->byte + 1);
	d->data = (UC *)malloc(size * sizeof(UC));

	b <<= bit;		// 提取模版
	for (i=0; i<size; i++)
	{
		for (j=0, tmp=0; j<8; j++)
		{
			tmp <<= 1;		// tmp 一次放八个像素的比特
			if ((s->data[top++] & b) != 0) tmp |= 1;
		}
		d->data[i] = tmp;
	}
	return 1;
}

// 将 d 像素的第 i 个比特修改为 s , 要求 s 是二值图像, 大小和 d 相等
int update_bit(IMG *d, IMG const *s, int bit)		// 合成比特
{
	if (d == NULL || s == NULL || s->data == NULL
		|| d->height != s->height 
		|| d->width != s->width
		|| s->flag != '4') return 0;

	int i, size;
	int top;		// s 的下标
	UC j = 0x80;	// 最右边置 1
	UC b1 = 1;		// 或后为 1
	UC b0;			// 与后为 0

	b1 <<= bit;		// 提取位的模版 0000 0001
	b0 = ~b1;		// 取反后       1111 1110
	size = d->height * d->width * d->byte;
	for (top=i=0, j=0x80; i<size; i++)
	{
		if (s->data[top] & j)
			d->data[i] |= b1;
		else 
			d->data[i] &= b0;
		j >>= 1;
		if (j == 0)		// 八个比特循环完了
		{
			j = 0x80;
			top++;
		}
	}
	return 1;
}

// 提取 RGB 三个分量到 r, g, b, 如果 r 有数据会被释放掉
int extract_rgb(IMG *ppm, IMG *r, IMG *g, IMG *b)
{
	if (ppm == NULL) return 0;

	if (r->data != NULL) free(r);	// 释放原来的
	if (g->data != NULL) free(g);
	if (b->data != NULL) free(b);
	*r = *g = *b = *ppm;
	r->byte = g->byte = b->byte = 1;
	r->flag = g->flag = b->flag = '5';

	int new_size = r->height * r->width * r->byte * sizeof(UC);
	r->data = (UC *)malloc(new_size);	// 分配空间
	g->data = (UC *)malloc(new_size);
	b->data = (UC *)malloc(new_size);

	int i, j, top;
	int ww = ppm->width * ppm->byte;
	for (i=0, top=0; i<ppm->height; i++)	// 分离
	{
		for (j=0; j<ww; j+=3, top++)
		{
			r->data[top] = ppm->data[i * ww + j];
			g->data[top] = ppm->data[i * ww + j + 1];
			b->data[top] = ppm->data[i * ww + j + 2];
		}
	}
	return 1;
}

// 合成 RGB 三个分量, 需确保 r, g, b, 大小一样
int combine_rgb(IMG *ppm, IMG *r, IMG *g, IMG *b)
{
	if (r == NULL || g == NULL || b == NULL) return 0;
	if (ppm->data) free(ppm->data);		// 释放原来的

	ppm->type = 'P';
	ppm->flag = '6';
	ppm->maxv = 255;
	ppm->height = r->height;
	ppm->width = r->width;
	ppm->byte = 3;
	ppm->data = (UC *)malloc(ppm->height 
		* ppm->width * ppm->byte * sizeof(UC));

	int i, j, top;
	int ww = ppm->width * ppm->byte;
	for (i=0, top=0; i<ppm->height; i++)	// 合成
	{
		for (j=0; j<ww; j+=3, top++)
		{
			ppm->data[i * ww + j] = r->data[top];
			ppm->data[i * ww + j + 1] = g->data[top];
			ppm->data[i * ww + j + 2] = b->data[top];
		}
	}
	return 1;
}

// 读取一张图片, 支持 p4, p5, p6 图
int open_img(IMG *ppm, char *path)
{
	FILE *fp = fopen(path, "rb");
	int ret = 1;	// 返回值
	int f = 1;
	int w, h, i, ww;

	if (fp != NULL)
	{
		ppm->type = fgetc(fp);
		ppm->flag = fgetc(fp);
		fgetc(fp);		// 多余空格
		fscanf(fp, "%d", &(ppm->width));
		fgetc(fp);
		fscanf(fp, "%d", &(ppm->height));
		fgetc(fp);
		if (ppm->flag == '5' || ppm->flag == '6')
		{
			fscanf(fp, "%d", &(ppm->maxv));
			fgetc(fp);
		}
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
		else if (ppm->flag == '4')
		{
			ppm->data = (UC *)malloc(h * (w / 8 + 1) * sizeof(UC));
			ppm->byte = 8;
		}
		else ret = 0;

		if (ret != 0 && ppm->data != NULL)	// 读取数据
		{
			if (ppm->flag == '5' || ppm->flag == '6')
				ww = w * ppm->byte;
			else if (ppm->flag == '4') 
				ww = ppm->width / ppm->byte;

			for (i=0, f=1; i<h && f!=0; i++)
			{
				// fread() 一次读入的数据有限, 这里分段读
				f = fread(ppm->data + i * ww, 1, ww, fp);
			}
			if (f == 0)		// 一部分数据读取失败
			{
				free(ppm->data);
				ret = 0;
			}
		}
		else ret = 0;
		fclose(fp);		// 关闭文件
	}
	else ret = 0;

	return ret;
}

// 写入一张图片, 支持 p4, p5, p6 图
int save_img(IMG *ppm, char *path)
{
	FILE *fp = fopen(path, "wb");
	int w = ppm->width;
	int h = ppm->height;
	int i, f, ww;
	int ret = 1;

	if (fp == NULL) return 0;

	if (ppm->flag == '5' || ppm->flag == '6')
	{
		// 写入 ppm 头
		fprintf(fp, "%c%c\n%d\n%d\n%d\n", 
			ppm->type, ppm->flag, w, h, ppm->maxv);

		// 写入图像数据
		ww = w * ppm->byte;
		for (i=0, f=1; i<h && f!=0; i++)
			f = fwrite(ppm->data + i * ww, 1, ww, fp);

		if (f == 0) ret = 0;	// 写入错误
	}
	else if (ppm->flag == '4')
	{
		// 写入 ppm 头
		fprintf(fp, "%c%c\n%d\n%d\n", ppm->type, ppm->flag, w, h);

		// 写入图像数据
		int ww = w / ppm->byte;
		for (i=0, f=1; i<h && f!=0; i++)
			f = fwrite(ppm->data + i * ww, 1, ww, fp);

		if (f == 0) ret = 0;	// 写入错误
	}
	else ret = 0;
	fclose(fp);		// 记得关闭

	return ret;
}

int copy_img(IMG *d, IMG const *s)		// 复制
{
	if (d == NULL || s == NULL) return 0;

	if (d->data != NULL) free(d->data);		// 释放原来的图像
	*d = *s;								// 复制成员
	int size = s->height * s->width;

	if (s->flag == '5' || s->flag == '6')
		size *= s->byte;
	else if (s->flag == '4')
		size = size / s->byte;

	d->data = (UC *)malloc(size * sizeof(UC));

	// 复制图像数据
	for (int i=0; i<size; i++) d->data[i] = s->data[i];

	return 0;
}

// 关闭一张图片
int close_img(IMG *ppm)
{
	if (ppm->data) free(ppm->data);
	memset(ppm, 0, sizeof(IMG));

	return 0;
}

int check(char *data, ULI size)	// 合法检查, 保证字母和数字
{
	if (strlen(data) >= size) return 0;
	while (*data != '\0') 
	{
		if (*data++ == '+') return 0;
	}
	return 1;
}
