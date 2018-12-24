#include <x86.h>

#define PG_ALIGN __attribute((aligned(PGSIZE)))

static PDE kpdirs[NR_PDE] PG_ALIGN;
static PTE kptabs[PMEM_SIZE / PGSIZE] PG_ALIGN;
static void* (*pgalloc_usr)(size_t);
static void (*pgfree_usr)(void*);

_Area segments[] = {      // Kernel memory mappings
  {.start = (void*)0,          .end = (void*)PMEM_SIZE}
};

#define NR_KSEG_MAP (sizeof(segments) / sizeof(segments[0]))
//准备一些内核页表
//用于进行VME相关的初始化操作. 其中它还接受两个来自操作系统的页面分配回调函数的指针, 让AM在必要的时候通过这两个回调函数来申请/释放一页物理页.
int _vme_init(void* (*pgalloc_f)(size_t), void (*pgfree_f)(void*)) {
  pgalloc_usr = pgalloc_f;
  pgfree_usr = pgfree_f;

  int i;

  // make all PDEs invalid
  for (i = 0; i < NR_PDE; i ++) {
    kpdirs[i] = 0;
  }

  PTE *ptab = kptabs;
  for (i = 0; i < NR_KSEG_MAP; i ++) {
    uint32_t pdir_idx = (uintptr_t)segments[i].start / (PGSIZE * NR_PTE);
    uint32_t pdir_idx_end = (uintptr_t)segments[i].end / (PGSIZE * NR_PTE);
    for (; pdir_idx < pdir_idx_end; pdir_idx ++) {
      // fill PDE
      kpdirs[pdir_idx] = (uintptr_t)ptab | PTE_P;

      // fill PTE
      PTE pte = PGADDR(pdir_idx, 0, 0) | PTE_P;
      PTE pte_end = PGADDR(pdir_idx + 1, 0, 0) | PTE_P;
      for (; pte < pte_end; pte += PGSIZE) {
        *ptab = pte;
        ptab ++;
      }
    }
  }

  set_cr3(kpdirs);
  set_cr0(get_cr0() | CR0_PG);

  return 0;
}
//用于创建一个默认的地址空间
int _protect(_Protect *p) {
  PDE *updir = (PDE*)(pgalloc_usr(1));
  p->pgsize = 4096;
  p->ptr = updir;
  // map kernel space
  for (int i = 0; i < NR_PDE; i ++) {
    updir[i] = kpdirs[i];
  }

  p->area.start = (void*)0x8000000;
  p->area.end = (void*)0xc0000000;
  return 0;
}
//用于销毁指定的地址空间
void _unprotect(_Protect *p) {
}

static _Protect *cur_as = NULL;
void get_cur_as(_Context *c) {
  c->prot = cur_as;
}

void _switch(_Context *c) {
  set_cr3(c->prot->ptr);
  cur_as = c->prot;
}
//它用于将地址空间p中虚拟地址va所在的虚拟页, 以prot的权限映射到pa所在的物理页. 当prot中的present位为0时, 表示让va的映射无效.
int _map(_Protect *p, void *va, void *pa, int mode) {
	if(!(mode&0x1))return -1;
	
	PDE* pt=(PDE*)p->ptr;//pt==page_table
	PDE* pde=&pt[PDX(va)];//get pde from page_table;

	if(!(*pde&PTE_P)){
		*pde=(uint32_t)pgalloc_usr(1)|PTE_P|PTE_W|PTE_U;	
	}

	PTE* pte=&((PTE*)PTE_ADDR(*pde))[PTX(va)];

	if(!(*pte&PTE_P)){
		*pte=(uint32_t)pa|PTE_P|PTE_W|PTE_U;
	}
  return 0;//参考了yzh在https://github.com/njuallen/nexus-am/blob/master/am/arch/x86-nemu/src/pte.c里面的实现
}
// 用于创建用户进程上下文. 我们之前已经介绍过这个API, 但加入虚存管理之后, 我们需要对这个API的实现进行一些改动, 具体改动会在下文介绍.
_Context *_ucontext(_Protect *p, _Area ustack, _Area kstack, void *entry, void *args) {
//  uint32_t* temp1=(uint32_t*)ustack.end;
//  printf("i am in ucontext\n");
 // *(--temp1)=0x0;//argc
//  *(--temp1)=0x0;//argv
//  *(--temp1)=0x0;//enc
// *(--temp1)=0x0;//ret_addr
  //assert(0);
//  return NULL;
  ustack.end-=4*sizeof(int);
  _Context* temp2=(_Context*)(ustack.end-sizeof(_Context));
  temp2->prot=p;
  temp2->edi=0;
  temp2->esi=0;
  temp2->ebp=0;
  temp2->esp=(uintptr_t)ustack.end;
  temp2->ebx=0;
  temp2->edx=0;
  temp2->ecx=0;
  temp2->eax=0;
  temp2->irq=0x81;
  temp2->err=0;
  temp2->eip=(uintptr_t)entry;
  temp2->cs=0x8;
  temp2->eflags=0x202;
//assert(0);
	
	
	
	return temp2;
}
