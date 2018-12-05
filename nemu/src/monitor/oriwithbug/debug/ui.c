#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void cpu_exec(uint64_t);

/* We use the `readline' library to provide more flexibility to read from stdin. */
char* rl_gets() {
  static char *line_read = NULL;

  if (line_read) {
    free(line_read);
    line_read = NULL;
  }

  line_read = readline("(nemu) ");

  if (line_read && *line_read) {
    add_history(line_read);
  }

  return line_read;
}

static int cmd_c(char *args) {
  cpu_exec(-1);
  return 0;
}

static int cmd_q(char *args) {
  return -1;
}

static int cmd_help(char *args);


static int cmd_step(char *args)
{
	char *arg=strtok(NULL," ");
	if(arg==NULL)
	{
		cpu_exec(1);
	}
	else
	{
	
	int N=1;
	sscanf(arg,"%d",&N);
	cpu_exec(N);
	}
	return 0;
}

static void show_register()
{	int i;
	
	for(i=R_EAX;i<=R_EDI;i++)
	{	
		printf("%s:0x%08x\n ",regsl[i],cpu.gpr[i]._32);	
	}
	printf("eip:0x%08x\n",cpu.eip);
	for(i=R_AX;i<=R_DI;i++)
	{
		printf("%s: 0x%08x\n",regsw[i],cpu.gpr[i]._16);
	}
	int j=0;
	for(i=R_AL;i<=R_BH;i=i+2)
	{	printf("%s:0x%08x\n",regsb[i],cpu.gpr[j]._8[0]);
		printf("%s:0x%08x\n",regsb[i+1],cpu.gpr[j]._8[1]);
		j++;

		}
}
WP*  Whereislist();
static void show_watch()
{
	WP* w=Whereislist();
	if(w==NULL)
	printf("There is no watchpoint,maybe you need to help w.\n");
	else{
	printf("NO:\tSTR:\tvalue:\n");
	while(w!=NULL)
	{
	printf("w[%d]\t%s\t%d | 0x%x\n",w->NO,w->str,w->value,w->value);
	w=w->next;	
	
	
	}	


	}

}
static int cmd_info(char *args)
{
	char *arg=strtok(NULL," ");
	switch(*arg)
{
	case 'r':show_register();return 0;
	case 'w':show_watch();return 0;
	
	default: printf("error");return 1; 
}


}
static int cmd_x(char *args)
{
	unsigned int ad,l,i;
/*char *len=strtok(NULL," ");
char *addr=strtok(NULL," ");
sscanf(len,"%u",&l);
printf("%u",l);
sscanf(addr,"0x%x",&ad);
printf("%x",ad);*/
	sscanf(args,"%u 0x%x",&l,&ad);
	printf("%u",l);
	printf("\n0x%x",ad);
	uint8_t temp;
	for(i=0;i<l;i++)
	{/*if(!(i&0xf))*/printf("\n0x%08x:",(ad+i));
		temp=pmem[ad+i];
		printf("0x%08x",(unsigned int)temp);
	//	printf("0x%02x",*(unsigned char *)hwa_to_va((ad+i)));
		}
		printf("\n");
	return 0;

}
static int cmd_p(char *args)
{
//	printf("in it!");
	bool Success;
//	printf("%s\n",args);
//	char *E=strtok(NULL," ");
	uint32_t result=expr(args,&Success);
//	printf("%s",args);
	if(Success==false)
	{	printf("\033[1;33m""Invaild input.\n""\033[0m");
		return 1;
	}
	else 
		printf("%d\n",result);
	return 0;
}

static int cmd_w(char *args)
{
	if(args==NULL)
	{
		printf("Are you kidding? Where is the expr?\n");
		return 0;}
	else
	{
		bool Success=0;
		uint32_t wresult=expr(args,&Success);
		if(Success)
		{
			new_wp(args,wresult);
			printf("You have added a watchpoint:%s=%d |0x%x\n",args,wresult,wresult);

		
		}
	else
	{
		printf("Some errors happen when devepoing the EXPR!\n");
	}		
	
	return 0;
	}
}
extern void free_wp(WP *wp);
static int cmd_d(char *args)
{
if(args==NULL)
{
printf("Are you kidding?Maybe you need to read help!\n");
return 0;

}
else{
int tempno;
tempno=atoi(args);
if(tempno>=32||tempno<0)
{
printf("Error of inputing!\n");
return 1;
}
else {

	
WP* w=Whereislist();
while(w->NO!=tempno)
{printf("%d",w->NO);
w=w->next;
if(w==NULL)
{
printf("No such watchpoint\n");
return 0;

}

}

free_wp(w);
printf("Successlly delete w[%d]:%s\n",w->NO,w->str);

};
return 0;


}



}


static struct {
  char *name;
  char *description;
  int (*handler) (char *);
} cmd_table [] = {
  { "help", "Display informations about all supported commands", cmd_help },
  { "c", "Continue the execution of the program", cmd_c },
  { "q", "Exit NEMU", cmd_q },
  
  /* TODO: Add more commands */
  {"si","step n or Single step if n==NULL",cmd_step},
  {"info","show you the informoation of register with r(register)",cmd_info},
  {"x","show you the information of the memory with EXPR_0x",cmd_x},
  {"p","do expression evalution through you commands",cmd_p},
  {"w","watch a expr therefore you can watch the change of it",cmd_w},
  {"d","delete the watchpoint as you want.",cmd_d},


};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_help(char *args) {
  /* extract the first argument */
  char *arg = strtok(NULL, " ");
  int i;

  if (arg == NULL) {
    /* no argument given */
    for (i = 0; i < NR_CMD; i ++) {
      printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
    }
  }
  else {
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(arg, cmd_table[i].name) == 0) {
        printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
        return 0;
      }
    }
    printf("Unknown command '%s'\n", arg);
  }
  return 0;
}

void ui_mainloop(int is_batch_mode) {
//  if (is_batch_mode) {
  //  cmd_c(NULL);
    //return;
 // }
//zhu shi le diyi ge if
//change
  while (1) {
    char *str = rl_gets();
    char *str_end = str + strlen(str);

    /* extract the first token as the command */
    char *cmd = strtok(str, " ");
    if (cmd == NULL) { continue; }

    /* treat the remaining string as the arguments,
     * which may need further parsing
     */
    char *args = cmd + strlen(cmd) + 1;
    if (args >= str_end) {
      args = NULL;
    }

#ifdef HAS_IOE
    extern void sdl_clear_event_queue(void);
    sdl_clear_event_queue();
#endif

    int i;
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(cmd, cmd_table[i].name) == 0) {
        if (cmd_table[i].handler(args) < 0) { return; }
        break;
      }
    }

    if (i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
  }
}
