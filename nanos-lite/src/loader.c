#include "proc.h"
#include <sys/types.h>
//#include "ramdisk.c"
//#define DEFAULT_ENTRY 0x4000000
#define DEFAULT_ENTRY 0x8048000
extern size_t get_ramdisk_size();
extern uint8_t ramdisk_start;
extern int fs_open(const char *pathname, int flags, int mode);
extern ssize_t fs_read(int fd, void *buf, size_t len); 
//size_t ramdisk_write(const void *buf,size_t offset,size_t len);
extern int fs_close(int fd);
extern size_t fs_filesz(int fd);
extern int _map(_Protect *p,void *va,void *pa,int mode);
size_t ramdisk_read(void *buf,size_t offset,size_t len);
static uintptr_t loader(PCB *pcb, const char *filename) {
  //TODO();
  Log("I am in loader,filename:%s",filename);
//	size_t filelen=get_ramdisk_size();
//char a[10000] ;
//filelen=ramdisk_read(&a,0,filelen);
//	filele=
//Log("I am after get_ramdisk");
//	filelen=ramdisk_read((void *)DEFAULT_ENTRY,0,filelen);
//	Log("I am pass ramdisk_read in loader");
//	size_t de=DEFAULT_ENTRY;
//	printf("%d\n",filelen);
//	printf("%x\n",(size_t)DEFAULT_ENTRY-ramdisk_start);
//	printf("%d\n",(size_t)de-ramdisk_start);

/*  int fd=fs_open(filename,0,0);
  Log("I am in loader,after filename:%s 0x%x",filename,filename);
	fs_read(fd,(void*)DEFAULT_ENTRY,fs_filesz(fd));
  Log("I am in loader,after read:%s 0x%x",filename,filename);
  Log("I am in loader,after read:%d 0x%x",*filename,filename);
	fs_close(fd);
  Log("I am in loader,after read:%d 0x%x",*filename,filename);
  return DEFAULT_ENTRY;*/
  //old above
  //
	int fd=fs_open(filename,0,0);
	uint32_t filesize=fs_filesz(fd);
	void* va=(void*)DEFAULT_ENTRY;
	//fs_read(fd,va,filesize);
	uint32_t pagenum=((filesize-1)>>12)+1;
	Log("filesize: :%d pagenum: %d",filesize,pagenum);
	while(pagenum--)
	{
		void* pa=new_page(1);
		_map(&pcb->as,va,pa,1);
		fs_read(fd,pa,PGSIZE);	
		va=va+PGSIZE;	
	
	}
	fs_close(fd);
	return DEFAULT_ENTRY;



}

void naive_uload(PCB *pcb, const char *filename) {
Log("I am in naive_uload: %s",filename);
  	uintptr_t entry = loader(pcb, filename);
Log("I am in naive_uload after: %s",filename);
  ((void(*)())entry) ();
}

void context_kload(PCB *pcb, void *entry) {
  _Area stack;
  stack.start = pcb->stack;
  stack.end = stack.start + sizeof(pcb->stack);

  pcb->cp = _kcontext(stack, entry, NULL);
}

void context_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);

  _Area stack;
  stack.start = pcb->stack;
  stack.end = stack.start + sizeof(pcb->stack);
Log("I am in context_uload");
  pcb->cp = _ucontext(&pcb->as, stack, stack, (void *)entry, NULL);
}
