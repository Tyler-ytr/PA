
//#include"nemu.h"
//#include<main.h>
#include"monitor/expr.h"
//uint32_t expr(char *e,bool*success);
int init_monitor(int, char *[]);
void ui_mainloop(int);

extern uint32_t expr(char *str,bool *success);
void woqu()
{
FILE *ffp=fopen("/home/ytr/ics2018/nemu/tools/gen-expr/input","r");

for(int i=0;i<84;i++)
{

char t_expr[2000];
char *t_buf;
assert (ffp!=NULL);
fgets(t_expr,6000,ffp);
uint32_t t_result=0;
//assert(0);
for(int k=0;t_expr[k]!=' ';k++)
{

t_result*=10;
t_result+=t_expr[k]-48;
}

t_buf=strtok(t_expr," ");
t_buf=strtok(NULL,"\n");
assert(t_buf!=NULL);
printf("%s\n",t_buf);
printf("%u\n",t_result);
bool* MUST =0;
uint32_t e_result=expr(t_buf,MUST);
//assert (0);
if(t_result!=e_result)
{
printf("wrong\n");
break;


}

}

}


int main(int argc, char *argv[]) {
  /* Initialize the monitor. */
	int is_batch_mode=init_monitor(argc,argv);
//	assert(0);
	woqu();
//	uint32_t expr(char *e,bool*success);
// uint32_t tresult;
// char tbuf[65535];
// char ttemp[65536];
 //bool tsuccess;
// uint32_t ttresult;
// bool*ts=&tsuccess;
//   	int is_batch_mode = init_monitor(argc, argv);
//FILE *ffp=fopen("..//tools/gen-expr/input","r");
//fscanf(ffp,"%u",&tresult);
//printf("yingyingying");
//fscanf(ffp,"%s",ttemp);
//fscanf(ffp,"%u%*[^\n]%*c%s",&tresult,ttemp);
//while(~fscanf){int i=0;
/*	
while(fgets(ttemp,65535,ffp)){
char* cresult=strtok(ttemp," ");
sscanf(cresult,"%d",&tresult);
ttresult=expr(tbuf,ts);
printf("trueresult:%d testresult: %d",tresult,ttresult);
if(ttresult!=tresult){
printf("wrong result of %s\n",tbuf);
break;
}

}*/
  /* Receive commands from user. */
  ui_mainloop(is_batch_mode);

  return 0;
}
