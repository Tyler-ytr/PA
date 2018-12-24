#include "memory.h"
#include "proc.h"
extern int _map(_Protect *p,void *va, void* pa,int mode);
static void *pf = NULL;

void* new_page(size_t nr_page) {
  void *p = pf;
  pf += PGSIZE * nr_page;
  assert(pf < (void *)_heap.end);
  return p;
}

void free_page(void *p) {
  panic("not implement yet");
}

/* The brk() system call handler. */
int mm_brk(uintptr_t new_brk) {
//	if(current->cur_brk==0)	{
//		current->cur_brk = current->max_brk = new_brk;
//	}
//	else{
		if(new_brk>current->max_brk){
			void* pgstart=(void*)PGROUNDUP(current->max_brk);
			void* pgend=(void*)PGROUNDUP(new_brk);//from memory.h
			void* va;
			void* pa;
			for(void* i=pgstart;i<=pgend;i+=PGSIZE){
				va=(void *)i;
				pa=new_page(1);
				_map(&current->as,va,pa,1);
			}
//			printf("max_brkbefore:0x%x\n",current->max_brk);
			current->max_brk=new_brk;
//			printf("max_after:0x%x\n",current->max_brk);
		}
		current->cur_brk=new_brk;		
//	}
  return 0;
}
//目前初始化MM的工作有两项, 第一项工作是将TRM提供的堆区起始地址作为空闲物理页的首地址, 将来会通过new_page()函数来分配空闲的物理页. 为了简化实现, MM中采用顺序的方式对物理页进行分配, 而且分配后无需回收. 第二项工作是调用AM的_vme_init()函数, 填写内核的页目录和页表, 然后设置CR3寄存器, 最后通过设置CR0寄存器来开启分页机制
void init_mm() {
  pf = (void *)PGROUNDUP((uintptr_t)_heap.start);
  Log("free physical pages starting from %p", pf);

  _vme_init(new_page, free_page);
}
