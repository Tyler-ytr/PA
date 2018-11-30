#include "proc.h"
//#include "ramdisk.c"
#define DEFAULT_ENTRY 0x4000000
extern size_t get_ramdisk_size();
extern uint8_t ramdisk_start;
size_t ramdisk_write(const void *buf,size_t offset,size_t len);
size_t ramdisk_read(void *buf,size_t offset,size_t len);
static uintptr_t loader(PCB *pcb, const char *filename) {
  //TODO();
  Log("I am in loader");
	size_t filelen=get_ramdisk_size();
//char a[10000] ;
//filelen=ramdisk_read(&a,0,filelen);
//	filele=
Log("I am after get_ramdisk");
	filelen=ramdisk_read((void *)DEFAULT_ENTRY,0,filelen);
	Log("I am pass ramdisk_read in loader");
//	size_t de=DEFAULT_ENTRY;
//	printf("%d\n",filelen);
//	printf("%x\n",(size_t)DEFAULT_ENTRY-ramdisk_start);
//	printf("%d\n",(size_t)de-ramdisk_start);
  return DEFAULT_ENTRY;
}

void naive_uload(PCB *pcb, const char *filename) {
Log("I am in naive_uload");
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
