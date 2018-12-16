#include <am.h>
#include <x86.h>
#include <klib.h>
static _Context* (*user_handler)(_Event, _Context*) = NULL;

void vecsys();
void vectrap();
void vecnull();

_Context* irq_handle(_Context *tf) {
	//assert(0);
	_Context *next = tf;
//	printf("tf:0x%x,eax:0x%x,ebx:0x%x,ecx:0x%x,edx:0x%x,esp:0x%x\n",tf,cpu.eax,cpu.ebx,cpu.ecx,cpu.edx,esp);
//	printf("tf:0x%x\n",tf);
/*	printf("tf->irq:0x%x\n",tf->irq);
	  printf("tf:0x%x\n",tf);
	  printf("tf->edi:0x%x\n",tf->edi); 
	      printf("tf->esi:0x%x\n",tf->esi);
	      printf("tf->ebp:0x%x\n",tf->ebp);
	      printf("tf->esp:0x%x\n",tf->esp);
	      printf("tf->edx:0x%x\n",tf->edx);
	      printf("tf->ebx:0x%x\n",tf->ebx);
	      printf("tf->ecx:0x%x\n",tf->ecx);
	      printf("tf->eax:0x%x\n",tf->eax);*/
	//
//	assert(0);
  if (user_handler) {
    _Event ev={0};
	
//	printf("tf->irq2:0x%x\n",tf->irq);
	//assert(0);
    switch (tf->irq) {
	  case 0x80: ev.event=_EVENT_SYSCALL;break;
	  case 0x81: ev.event=_EVENT_YIELD;	break;
      default: ev.event = _EVENT_ERROR; break;
    }
	
	//assert(0);
    next = user_handler(ev, tf);
//	assert(0);
    if (next == NULL) {
      next = tf;
    }
	//printf("0x%x\n",tf);
  }
//	assert(0);
//	printf("0x%x\n",tf);

  return next;
}

static GateDesc idt[NR_IRQ];

int _cte_init(_Context*(*handler)(_Event, _Context*)) {
  // initialize IDT
  for (unsigned int i = 0; i < NR_IRQ; i ++) {
    idt[i] = GATE(STS_TG32, KSEL(SEG_KCODE), vecnull, DPL_KERN);
  }

  // -------------------- system call --------------------------
  idt[0x81] = GATE(STS_TG32, KSEL(SEG_KCODE), vectrap, DPL_KERN);
  idt[0x80] = GATE(STS_TG32, KSEL(SEG_KCODE), vecsys, DPL_USER);

  set_idt(idt, sizeof(idt));

  // register event handler
  user_handler = handler;

  return 0;
}

_Context *_kcontext(_Area stack, void (*entry)(void *), void *arg) {
/*	_Context* temp_c=stack.end;
	temp_c.eflags=0x2;
	temp_c.cs=0x8;
*/
/*	uintptr_t *temp_stack=(uintptr_t*)stack.end;
   	*(--temp_stack)=0x2;//eflags
   	*(--temp_stack)=0x8;//cs
   	*(--temp_stack)=0x81;//eip
   	*(--temp_stack)=0x0;//err
   	*(--temp_stack)=0x0;//irq
   	*(--temp_stack)=0x0;//eax
   	*(--temp_stack)=0x0;//ecx
   	*(--temp_stack)=0x0;//ebx
   	*(--temp_stack)=0x0;//esp
   	*(--temp_stack)=0x0;//ebp
   	*(--temp_stack)=0x0;//esi
   	*(--temp_stack)=0x0;//edi
*/
	_Context*temp_c=(_Context*)(stack.end-sizeof(_Context));
	temp_c->edi=0;
	temp_c->esi=0;
	temp_c->ebp=(uintptr_t)stack.end;
	temp_c->esp=0;
	temp_c->ebx=0;
	temp_c->edx=0;
	temp_c->ecx=0;
	temp_c->eax=0;
	temp_c->irq=0x81;
	temp_c->err=0;
	temp_c->eip=(uintptr_t)entry;
	temp_c->cs=0x8;
	temp_c->eflags=0x0;//0x2|FL_IF
	printf("in kcontext:0x%x",(uintptr_t)entry);
//maywrong!!!
	return temp_c;
}

void _yield() {
  asm volatile("int $0x81");
}

int _intr_read() {
  return 0;
}

void _intr_write(int enable) {
}
