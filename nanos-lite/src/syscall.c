#include "common.h"
#include "syscall.h"

_Context* do_syscall(_Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;//eax
  a[1] = c->GPR2;//ebx
  a[2] = c->GPR3;//ecx
  a[3] = c->GPR4;//edx
//Log("I am in syscall");
  switch (a[0]) {
	  case SYS_yield:_yield();return 0; 
    default: panic("Unhandled syscall ID = %d", a[0]);
  }

  return NULL;
}
