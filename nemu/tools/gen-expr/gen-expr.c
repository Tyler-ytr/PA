#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>

// this should be enough
static char buf[65536];


static inline uint32_t choose(int n)
{
	srand((unsigned)time(NULL));
	uint32_t i=(uint32_t)(rand()%(n));
	return i;
}
static inline uint32_t chooseoff(int n)
{
	srand((unsigned)time(NULL));
	uint32_t i=(uint32_t)(rand()%n)+1;
	return i;
}



static inline void gen_num(int *p)
{
	uint32_t ran=choose(32767);
	if(ran==0&&buf[*p-1]=='/')
	{
	ran=chooseoff(32767);
	}
	buf[*p]=ran;
	buf[*p+1]='\0';
	*p=*p+1;
}
static inline void gen_rand_op(int *p)
{
	int k=choose(4);
	switch(k)
	{
		case 0:buf[*p]='+';
		case 1:buf[*p]='-';
		case 2:buf[*p]='*';
		case 3:buf[*p]='/';
		default :assert(0);
	}	
	buf[*p+1]='\0';
	*p=*p+1;

}
static inline void gen(char p,int *p)
{
	buf[*p]=p;
	*p=*p+1;
}



static inline void gen_rand_expr() {
 // buf[0] = '\0';
 int place=0;
 int *P=&place;
 if(place<65500)
  switch(choose(3))
  {
	case 0:gen_num(P);break;
	case 1:gen('(',P);gen_rand_expr();gen(')',P);break;
	default :gen_rand_expr();gen_rand_op(P);gen_rand_expr();break;  
  
  }
 else return;
}

static char code_buf[65536];
static char *code_format =
"#include <stdio.h>\n"
"int main() { "
"  unsigned result = %s; "
"  printf(\"%%u\", result); "
"  return 0; "
"}";

int main(int argc, char *argv[]) {
  int seed = time(0);
  srand(seed);
  int loop = 1;
  if (argc > 1) {
    sscanf(argv[1], "%d", &loop);
  }
  int i;
  for (i = 0; i < loop; i ++) {
    gen_rand_expr();

    sprintf(code_buf, code_format, buf);

    FILE *fp = fopen(".code.c", "w");
    assert(fp != NULL);
    fputs(code_buf, fp);
    fclose(fp);

    int ret = system("gcc .code.c -o .expr");
    if (ret != 0) continue;

    fp = popen("./.expr", "r");
    assert(fp != NULL);

    int result;
    fscanf(fp, "%d", &result);
    pclose(fp);

    printf("%u %s\n", result, buf);
  }
  return 0;
}
