#include "common.h"
#include "syscall.h"

_Context* do_syscall(_Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;//eax
  a[1] = c->GPR2;//ebx
  a[2] = c->GPR3;//ecx
  a[3] = c->GPR4;//edx
Log("I am in syscall");
printf("a0(eax): 0x%x\n",a[0]);
printf("a0(ebx): 0x%x\n",a[1]);
printf("a0(ecx): 0x%x\n",a[2]);
printf("a0(edx): 0x%x\n",a[3]);
  switch (a[0]) {
	  case SYS_exit:Log("I am before halt");_halt(a[1]);					//ID=0
	  case SYS_yield:_yield();Log("I experienced yield");return 0; //ID=1
	 
    default: panic("Unhandled syscall ID = %d", a[0]);
  }

  return NULL;
}
