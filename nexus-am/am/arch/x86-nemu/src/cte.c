#include <am.h>
#include <x86.h>
#include <klib.h>
static _Context* (*user_handler)(_Event, _Context*) = NULL;

void vectrap();
void vecnull();

_Context* irq_handle(_Context *tf) {
	//assert(0);
	_Context *next = tf;
	printf("0x%x\n",tf);
//	assert(0);
  if (user_handler) {
    _Event ev;
	
	//assert(0);
    switch (tf->irq) {
      default: ev.event = _EVENT_ERROR; break;
    }
	
	//assert(0);
    next = user_handler(ev, tf);
	assert(0);
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

  set_idt(idt, sizeof(idt));

  // register event handler
  user_handler = handler;

  return 0;
}

_Context *_kcontext(_Area stack, void (*entry)(void *), void *arg) {
  return NULL;
}

void _yield() {
  asm volatile("int $0x81");
}

int _intr_read() {
  return 0;
}

void _intr_write(int enable) {
}
