#include "common.h"
#include "syscall.h"
#include "fs.h"
#include "proc.h"
//extern int fs_open(const char*pathname,int flags,int mode);
int sys_yield();
extern void naive_uload(PCB *pcb,const char *filename);
int sys_write(int fd,const void*buf,size_t len);
_Context* do_syscall(_Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;//eax
  a[1] = c->GPR2;//ebx
  a[2] = c->GPR3;//ecx
  a[3] = c->GPR4;//edx
/*Log("I am in syscall");
    printf("c ->irq:0x%x\n",c->irq);                                                  
	printf(":0x%x\n",c); 
	printf("c->edi:0x%x\n",c->edi);                                               
    printf("c->esi:0x%x\n",c->esi);                                       
 	printf("c->ebp:0x%x\n",c->ebp);             
	printf("c->esp:0x%x\n",c->esp);								
	printf("c->edx:0x%x\n",c->edx);                                                       
	printf("c->ebx:0x%x\n",c->ebx);                                        
	printf("c->ecx:0x%x\n",c->ecx);               
	printf("c->eax:0x%x\n",c->eax); 
   printf("\n");	
printf("a0(eax): 0x%x\n",a[0]);
printf("a1(ebx): 0x%x\n",a[1]);
printf("a2(ecx): 0x%x\n",a[2]);
printf("a3(edx): 0x%x\n",a[3]);*/
  switch (a[0]) {
	  case SYS_exit://_halt(a[1]);	
					naive_uload(NULL,"/bin/init");break;
		  //ID=0 it should be a[1] and the result of that is I can't go through the dummy. the result of it is ebx=0x1.it has fixed.
	  case SYS_yield:{c->GPRx=sys_yield();
/*						 
printf("a0(eax): 0x%x\n",a[0]);
printf("a1(ebx): 0x%x\n",a[1]);
printf("a2(ecx): 0x%x\n",a[2]);
printf("a3(edx): 0x%x\n",a[3]);*/
						 break; //ID=1
					
					 }
		case SYS_open://Log("wait for sysopen");assert(0);
					  c->GPRx=fs_open((char*)a[1],(int)a[2],(int)a[3]);break;
		case SYS_read:{
						  //Log("wait for sysread");assert(0);
					  
					 c->GPRx=fs_read((int)a[1],(void*)a[2],(size_t)a[3]);break; 
					  
					  
					  }
		case SYS_write:{//Log("I am in write");
					//	   int len=a[3];
					//	   void*buf=(void *)a[2];
					//	   if(a[1]==1||a[1]==2)
					//	   {					  for(int i = 0; i < len; i++) {
					//						_putc(((char*)buf)[i]);
					//								}}
					c->GPRx=sys_write(a[1],(void*)a[2],a[3]);
							break;	  
					  Log("wait for syswrite");assert(0);}
		
		case SYS_kill:Log("wait for syskill");assert(0);
		case SYS_getpid:Log("wait for sysgetpid");assert(0);
		case SYS_close:{//Log("wait for sysclose");assert(0);
					   c->GPRx=fs_close((int)a[1]);
					   }
		case SYS_lseek:{//Log("wait for syslseek");
						  // assert(0);
					   c->GPRx=fs_lseek((int)a[1],(off_t)a[2],(int)a[3]);break;}
		case SYS_brk:{
					 
			c->GPR1=0;	 
				//	return 0; 
				break;
					 Log("wait for sysbrk");assert(0);
					 }
		
		case SYS_fstat:Log("wait for syslfstat");assert(0);
		case SYS_time:Log("wait for sysltime");assert(0);
		case SYS_signal:Log("wait for syssignal");assert(0);
		case SYS_execve:{
						
						Log("wait for sysexecve");assert(0);
						Log("sys_execve: name=%s",(char*)a[1]);
						naive_uload(NULL,(char*)a[1]);
						break;
						}
		case SYS_fork:Log("wait for sysfork");assert(0);
		case SYS_link:Log("wait for syslink");assert(0);
		case SYS_unlink:Log("wait for sysunlink");assert(0);
		case SYS_wait:Log("wait for syswait");assert(0);
		case SYS_times:Log("wait for systime");assert(0);
		case SYS_gettimeofday:Log("wait for sysgettimeofday");assert(0);
		
		default: panic("Unhandled syscall ID = %d", a[0]);
  } 

  return NULL;
}

int sys_yield()
{
	_yield();
	return 0;
}

int sys_write(int fd,const void*buf,size_t len)
{
	int i=0;
	if(fd==1||fd==2)
	{
		for(;i<len;i++)
		{
			_putc(((char*)buf)[i]);
		}
	}	
	else
	{
		return fs_write(fd,buf,len);
	}

return len;



}
