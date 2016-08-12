
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

const int table_len = 62;
const char table[table_len + 1] = {
'a','b','c','d','e','f','g','h','i','j','k','l',
'm','n','o','p','q','r','s','t','u','v','w','x',
'y','z','A','B','C','D','E','F','G','H','I','J',
'K','L','M','N','O','P','Q','R','S','T','U','V',
'W','X','Y','Z','0','1','2','3','4','5','6','7',
'8','9'};

// 返回字符在 table 中的下标
inline int table_pos(char a)
{
	if ('a' <= a && a <='z')
		return a - 'a';
	else if ('A' <= a && a <='Z')
		return a - 'A' + 26;
	else if ('0' <= a && a <='9')
		return a - '0' + 52;
	else return -1;			// -1 发生错误
}

// 密码结构:
// 密码长度 32 位, 前两位干扰码
// 第三位是密钥, 第四位是密文长度 len
// 每个 2 个密文中插入一个干扰码, 改进成每隔 k 个
// 对明文加上密钥变成密文
void encode(char *m , char *d)		// 加密
{
	int m_len = strlen(m);
	int p_key = rand() % 51 + 5;	// 密钥 [5-55]
	char *s = d;
	int top = 4;
	int i, pos;

	d[0] = table[rand() % table_len];	// 干扰码
	d[1] = table[rand() % table_len];
	d[2] = table[p_key];				// 密钥
	d[3] = table[m_len * 2 - 1];		// 长度
	for (i=0; i<m_len; i++)			// 变换明文
	{
		if (i % 2 == 0)
			s[top++] = table[rand() % table_len];
		pos = (table_pos(m[i]) + p_key) % table_len;
		s[top++] = table[pos];
	}
	while (top < 32)			// 填满干扰码
		s[top++] = table[rand() % table_len];
	d[32] = '\0';
}

// 解密过程是加密的逆运算
int decode(char *d, char *m)
{
	int m_len = (table_pos(d[3]) + 1) / 2;	// 提取长度
	int p_key = table_pos(d[2]);			// 提取密钥
	char *s = d + 4;
	int i, pos;

	for (i=0; i<m_len; i++)
	{
		if (i % 2 == 0) s++;
		pos = table_pos(*s++);			// 寻找下标
		if (pos == -1) return 0;		// 密码结构错误
		m[i] = table[(pos - p_key + table_len) % table_len];
	}
	m[i] = '\0';
	return 1;
}

int main(void)
{
	char m[35];		// 明文
	char d[35];		// 密文

	srand(time(NULL));
	while (1)
	{
		printf("\n输入英文+数字(4-16)位: ");
		scanf("%s", m);
		encode(m, d);
		printf("密文: %s\n", d);
		m[0] = '\0';
		if (decode(d, m))
			printf("明文: %s\n", m);
		else printf("密码错误!\n");
	}
	return 0;
}