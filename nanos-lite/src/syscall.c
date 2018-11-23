#include "common.h"
#include "syscall.h"

_Context* do_syscall(_Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;//eax
  a[1] = c->GPR2;//ebx
  a[2] = c->GPR3;//ecx
  a[3] = c->GPR4;//edx
Log("I am in syscall");
    printf("c ->irq:0x%x\n",c->irq);                                                  
	printf(":0x%x\n",c); 
	printf("c->edi:0x%x\n",c->edi);                                               
    printf("c->esi:0x%x\n",c->esi);                                       
 	printf("c->ebp:0x%x\n",c->ebp);             
	printf("c->esp:0x%x\n",c->esp);								
	printf("c->edx:0x%x\n",c->edx);                                                       
	printf("c->ebx:0x%x\n",c->ebx);                                        
	printf("c->ecx:0x%x\n",c->ecx);               
	printf("c->eax:0x%x\n",c->eax); 
   printf("\n");	
printf("a0(eax): 0x%x\n",a[0]);
printf("a1(ebx): 0x%x\n",a[1]);
printf("a2(ecx): 0x%x\n",a[2]);
printf("a3(edx): 0x%x\n",a[3]);
  switch (a[0]) {
	  case SYS_exit:Log("I am before halt");_halt(a[1]);					//ID=0 it should be a[1] and the result of that is I can't go through the dummy. the result of it is ebx=0x1
	  case SYS_yield:c->GPR1=1;Log("I experienced yield");break; //ID=1
	 
    default: panic("Unhandled syscall ID = %d", a[0]);
  }

  return NULL;
}
