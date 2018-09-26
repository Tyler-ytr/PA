
//#include"nemu.h"
//#include<main.h>
#include"monitor/expr.h"

int init_monitor(int, char *[]);
void ui_mainloop(int);

int main(int argc, char *argv[]) {
  /* Initialize the monitor. */
	int is_batch_mode=init_monitor(argc,argv);
 uint32_t tresult[1000];
 char tbuf[65535];
 bool tsuccess;
 uint32_t ttresult;
 bool*ts=&tsuccess;
//   	int is_batch_mode = init_monitor(argc, argv);
FILE *ffp=fopen("..//tools/gen-expr/input","r");
//while(~fscanf){int i=0;
int i=0;	
while(fscanf(ffp,"%u%*[^\n]%*c%s",&tresult[i],tbuf)){

ttresult=expr(tbuf,ts);
printf("trueresult:%d testresult: %d",tresult[i],ttresult);
if(ttresult!=tresult[i]){
printf("wrong result of %s\n",tbuf);
break;
}i++;
}
  /* Receive commands from user. */
  ui_mainloop(is_batch_mode);

  return 0;
}
