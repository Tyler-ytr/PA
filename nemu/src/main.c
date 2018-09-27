
//#include"nemu.h"
//#include<main.h>
#include"monitor/expr.h"
//uint32_t expr(char *e,bool*success);
int init_monitor(int, char *[]);
void ui_mainloop(int);

int main(int argc, char *argv[]) {
  /* Initialize the monitor. */
	int is_batch_mode=init_monitor(argc,argv);
	uint32_t expr(char *e,bool*success);
 uint32_t tresult;
// char tbuf[65535];
 char ttemp[65536];
 //bool tsuccess;
// uint32_t ttresult;
// bool*ts=&tsuccess;
//   	int is_batch_mode = init_monitor(argc, argv);
FILE *ffp=fopen("..//tools/gen-expr/input","r");
fscanf(ffp,"%u%*[^\n]%*c%s",&tresult,ttemp);
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
