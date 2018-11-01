#include "trap.h"

char *s[] = {
	"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", 
	"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaab",
	"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
	", World!\n",
	"Hello, World!\n",
	"#####"
};

char str1[] = "Hello";
char str[20];
//char a[3]="mn";
int main() {
//	_putc(a[0]);
	nemu_assert(strcmp(s[0], s[2]) == 0);
//	char b=a;
//	_putc(a[0]);
	nemu_assert(strcmp(s[0], s[1]) == -1);
//	_putc(a[0]);
	nemu_assert(strcmp(s[0] + 1, s[1] + 1) == -1);
//	_putc(a[0]);
	nemu_assert(strcmp(s[0] + 2, s[1] + 2) == -1);
//	_putc(a[0]);
	nemu_assert(strcmp(s[0] + 3, s[1] + 3) == -1);
//	_putc(a[0]);

	nemu_assert(strcmp( strcat(strcpy(str, str1), s[3]), s[4]) == 0);
//	_putc(a[1]);

	nemu_assert(memcmp(memset(str, '#', 5), s[5], 5) == 0);
//	_putc(a[1]);

	return 0;
}
