#include <stdio.h>

// max 不断累积子段和, 若 max 为负了则重新计数
int dp(int *num, int n, int *s, int *e)
{
	int max = 0;
	int sum = 0;
	int i;
	
	*e = *s = 0;
	for (i=0; i<n; i++)
	{
		if (max >= 0) max += num[i];
		else { max = num[i];   *s = i; }
		if (max >= sum) { sum = max;  *e = i; }
	}
	return sum;
}

int main(void)
{
	int num[200];
	int n, i;
	int start, end;
	int sum;

	printf("输入 n, 及 n 个数字: ");
	scanf("%d", &n);
	for (i=0; i<n; i++) scanf("%d", num + i);
	sum = dp(num, n, &start, &end);
	printf("\n最大子段和为: %d\n子段: ", sum);
	if (start >= end) printf("无");
	else 
		for (i=start; i<=end; i++)
			printf("%d, ", num[i]);	
	printf("\n");
	return 0;
}