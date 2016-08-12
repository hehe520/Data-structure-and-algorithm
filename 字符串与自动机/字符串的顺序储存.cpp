#include <stdio.h>

#define MAX 100
// ´®µÄË³Ðò´¢´æ
typedef char string[MAX + 1];

char *strcpy(char *aim, char *re)
{
	while (*re != '\0') *aim++ = *re++;
	*aim = '\0';
	return aim;
}

char *strcat(char *a, char *b)
{
	while (*a != '\0') a++;
	while (*b != '\0') *a++ = *b++;
	*a = '\0';

	return a;
}

int main(void)
{
	string s1, s2;

	strcpy(s1, "abcdefg");
	strcpy(s2, "kunkunkun");
	strcat(s1, s2);
	puts(s1);

	return 0;
}
