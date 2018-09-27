
//#include"nemu.h"
//#include<main.h>
#include"monitor/expr.h"
//uint32_t expr(char *e,bool*success);
int init_monitor(int, char *[]);
void ui_mainloop(int);


void woqu()
{
FILE *ffp=fopen("..//tools/gen-expr/inout","r");
for(int i=0;i<100;i++)
{

char t_expr[2000];
char *t_buf;
fgets(t_expr,5000,ffp);
uint32_t t_result=0;
for(int i=0;t_expr[i]!=' ';i++)
{

t_result*=10;
t_result+=t_expr[i]-48;


}

t_buf=strtok(t_expr," ");
t_buf=strtok(NULL,"\n");

bool* MUST =0;
uint32_t e_result=expr(t_buf,MUST);
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
