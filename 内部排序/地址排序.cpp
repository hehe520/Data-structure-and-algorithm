#include <stdio.h>

// 对于大型的结构, 不便于交换, 于是就交换他们的地址

typedef struct		// 假设这个结构很大
{
	int key;
	int other1;
	char *other2;
}keytype;

// 对地址进行排序, 其实还是用别的排序办法, 这里用插入
void addr_sort(keytype *add[], int n)
{
	int i, j;
	keytype *tmp;

	for (i=1; i<n; i++)
	{
		if (add[i-1]->key > add[i]->key)
		{
			tmp = add[i];
			for (j=i; j>0; j--)
			{
				if (add[j-1]->key > tmp->key)
					add[j] = add[j-1];
				else break;
			}
			add[j] = tmp;
		}
	}
}

int main(void)
{
	int i, n;
	keytype data[100];
	keytype *add[100];

	printf("请输入 n, 及 n 个元素: ");
	scanf("%d", &n);
	for (i=0; i<n; i++) 
	{
		add[i] = &data[i];		// 记录地址
		scanf("%d", &data[i].key);
	}
	addr_sort(add, n);		// 对地址排序
	printf("排序后: ");
	for (i=0; i<n; i++) printf("%d, ", add[i]->key);
	printf("\n\n");
	return 0;
}