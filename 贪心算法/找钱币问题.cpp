#include <stdio.h>

// 钱币有 1 元, 5 元, 10 元, 钱币必须是倍数关系, 否则不能用贪心算法
// 要求找出钱币的数量最少

void zhao(int *y, int *wu, int *shi, int pay)
{
	(*y) = (*wu) = (*shi) = 0;
	while (pay >= 10) { (*shi)++;   pay -= 10; }
	while (pay >= 5) { (*wu)++;   pay -= 5; }
	(*y) += pay;
}

int main(void)
{
	int pay;
	int yi, wu, shi;

	printf("输入要找出的钱数: ");
	scanf("%d", &pay);
	zhao(&yi, &wu, &shi, pay);
	printf("\n找 %d 个一元, %d 个五元, %d 个十元\n", yi, wu, shi);
	printf("共找 %d 个钱币\n", yi + wu + shi);

	return 0;
}