#include "proc.h"

#define MAX_NR_PROC 4
extern void naive_uload(PCB *pcb,const char *filename);
static PCB pcb[MAX_NR_PROC] __attribute__((used));
static PCB pcb_boot;
PCB *current;

void switch_boot_pcb() {
  current = &pcb_boot;
}

void hello_fun(void *arg) {
  int j = 1;
  while (1) {
    Log("Hello World from Nanos-lite for the %dth time!", j);
    j ++;
    _yield();
  }
}

void init_proc() {
	Log("I am in proc");
//naive_uload(NULL,"/bin/init");
  context_kload(&pcb[1], "/bin/init");
// context_kload(&pcb[0], (void *)hello_fun);  
    switch_boot_pcb();
}

_Context* schedule(_Context *prev) {
 // save the context pointer
Log("I am in schedule"); 
printf("prev->edi:0x%x\n",prev->edi);
printf("prev->esi:0x%x\n",prev->esi);
printf("prev->ebp:0x%x\n",prev->ebp);
printf("prev->esp:0x%x\n",prev->esp);
printf("prev->ebx:0x%x\n",prev->ebx);
printf("prev->edx:0x%x\n",prev->edx);
printf("prev->ecx:0x%x\n",prev->ecx);
printf("prev->eax:0x%x\n",prev->eax);
printf("prev->irq:0x%x\n",prev->irq);
printf("prev->err:0x%x\n",prev->err);
printf("prev->eip:0x%x\n",prev->eip);
printf("prev->cs:0x%x\n",prev->cs);
printf("prev->eflags:0x%x\n",prev->eflags);
  current->cp = prev;
 
  // always select pcb[0] as the new process
  //current = &pcb[0];
current = (current == &pcb[0] ? &pcb[1] : &pcb[0]); 
  // then return the new context
  return current->cp; 
	
	
	
//	return NULL;
}
