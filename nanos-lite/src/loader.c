#include "proc.h"
//#include "ramdisk.c"
#define DEFAULT_ENTRY 0x4000000
extern size_t get_ramdisk_size();
extern uint8_t ramdisk_start;
size_t ramdisk_write(const void *buf,size_t offset,size_t len);
size_t ramdisk_read(void *buf,size_t offset,size_t len);
static uintptr_t loader(PCB *pcb, const char *filename) {
//  TODO();a
	size_t filelen=get_ramdisk_size();
char a[10000] ;
filelen=ramdisk_read(&a,0,filelen);
//	filelen=
	filelen=ramdisk_write(&a,(size_t)DEFAULT_ENTRY-ramdisk_start,filelen);
	printf("%d\n",filelen);
  return DEFAULT_ENTRY;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  ((void(*)())entry) ();
}

void context_kload(PCB *pcb, void *entry) {
  _Area stack;
  stack.start = pcb->stack;
  stack.end = stack.start + sizeof(pcb->stack);

  pcb->tf = _kcontext(stack, entry, NULL);
}

void context_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);

  _Area stack;
  stack.start = pcb->stack;
  stack.end = stack.start + sizeof(pcb->stack);

  pcb->tf = _ucontext(&pcb->as, stack, stack, (void *)entry, NULL);
}
