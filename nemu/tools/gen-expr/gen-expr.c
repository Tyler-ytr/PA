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
//	srand((unsigned)time(NULL));
	int i=(uint32_t)(rand()%(n));
	return i;
}
static inline uint32_t chooseoff(int n)
{
//	srand((unsigned)time(NULL));
	uint32_t i=(uint32_t)(rand()%n)+1;
	return i;
}



static inline void gen_num(int *p)
{   int c=0;
	uint32_t lens=choose(10)+1;
	char find[10]={'0','1','2','3','4','5','6','7','8','9'};
	int flag=0;
	if(lens==1)flag=1;
	for(int i=0;i<lens;i++)
	{	
	c=choose(10);
	if(c==0&&(*p==0||buf[*p-1]=='+'||buf[*p-1]=='-'||buf[*p-1]=='*'||buf[*p-1]=='/'||buf[*p-1]=='(')){
	
	continue;
	}
	buf[*p]=find[c];
	*p=*p+1;
	buf[*p]='\0';
	}	



//	buf[*p]=ran;
//	buf[*p+1]='\0';

}
static inline void gen_rand_op(int *p)
{
	unsigned int k=rand()%3;
	switch(k)
	{
	//	case 0:buf[*p]='+';
		case 1:buf[*p]='-';break;
		case 2:buf[*p]='*';break;
		case 3:buf[*p]='/';break;
		default:buf[*p]='+';break;
	}	
	buf[*p+1]='\0';
	*p=*p+1;

}
static inline void gen(char t,int *p)
{
	buf[*p]=t;
	*p=*p+1;
}



static inline void gen_rand_expr(int *P) {
 // buf[0] = '\0';
// int place=0;
// int *P=&place;

  switch(choose(3))
  {
	case 0:gen_num(P);break;
	case 1:gen('(',P);gen_rand_expr(P);gen(')',P);break;
	default :gen_rand_expr(P);gen_rand_op(P);gen_rand_expr(P);break;  
  
  }
 
}

static char code_buf[65536];
static char *code_format =
"#include <stdio.h>\n"
"int main() { "
"  unsigned result=%s; "
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
  int place=0;
  int *p=&place;
  for (i = 0; i < loop; i ++) {
	if(place<50000)
  	  gen_rand_expr(p);

    sprintf(code_buf, code_format, buf);//insert buf into code_buf

    FILE *fp = fopen(".code.c", "w");//creat c.code for writing in
    assert(fp != NULL);
    fputs(code_buf, fp);
    fclose(fp);//close the fp

    int ret = system("gcc .code.c -o .expr");//call the system to create .expr based on code.c
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
