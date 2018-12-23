#include "memory.h"

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
  return 0;
}
//目前初始化MM的工作有两项, 第一项工作是将TRM提供的堆区起始地址作为空闲物理页的首地址, 将来会通过new_page()函数来分配空闲的物理页. 为了简化实现, MM中采用顺序的方式对物理页进行分配, 而且分配后无需回收. 第二项工作是调用AM的_vme_init()函数, 填写内核的页目录和页表, 然后设置CR3寄存器, 最后通过设置CR0寄存器来开启分页机制
void init_mm() {
  pf = (void *)PGROUNDUP((uintptr_t)_heap.start);
  Log("free physical pages starting from %p", pf);

  _vme_init(new_page, free_page);
}
