#include "common.h"
extern _Context* do_syscall(_Context *c);
static _Context* do_event(_Event e, _Context* c) {
//	Log("I am in do event");
/*	      printf("c ->irq:0x%x\n",c->irq);        
	       printf("c:0x%x\n",c);
	        printf("c->edi:0x%x\n",c->edi);
	         printf("c->esi:0x%x\n",c->esi);
		      printf("c->ebp:0x%x\n",c->ebp);
		       printf("c->esp:0x%x\n",c->esp);
		       printf("c->edx:0x%x\n",c->edx);
		         printf("c->ebx:0x%x\n",c->ebx);
			      printf("c->ecx:0x%x\n",c->ecx);
			       printf("c->eax:0x%x\n",c->eax);
			       printf("\n");
*/
  switch (e.event) {
		case _EVENT_YIELD:{//Log("I am in EVENT_YIELD event");
						return schedule(c);  
//Log("after shedule");						  
					//	  break;
					  }
		case _EVENT_SYSCALL:return do_syscall(c);
		case _EVENT_IRQ_TIMER:Log("I am in _EVENT_IRQ_TIMER"); _yield();
	
    default: panic("Unhandled event ID = %d", e.event);
  }

  return NULL;
}

void init_irq(void) {
  Log("Initializing interrupt/exception handler...");
  _cte_init(do_event);
}
