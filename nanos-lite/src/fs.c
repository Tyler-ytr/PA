#include "fs.h"
#include <sys/types.h>
extern size_t fb_write(const void *buf,size_t offset,size_t len);
extern size_t dispinfo_read(void *buf,size_t offset,size_t len);
typedef size_t (*ReadFn) (void *buf, size_t offset, size_t len);
typedef size_t (*WriteFn) (const void *buf, size_t offset, size_t len);
extern size_t serial_write(const void *buf,size_t offset,size_t len);
typedef struct {
  char *name;
  size_t size;
  size_t disk_offset;
  size_t open_offset;
  ReadFn read;
  WriteFn write;
} Finfo;

enum {FD_STDIN, FD_STDOUT, FD_STDERR, FD_FB,FD_EVENTS,FD_DISPINFO};
extern int screen_width();
extern int screen_height();
extern size_t ramdisk_read(void *buf,size_t offset,size_t len);
extern size_t ramdisk_write(const void *buf,size_t offset,size_t len);
int fs_open(const char*pathname,int flags,int mode);
ssize_t fs_read(int fd,void *buf,size_t len);
ssize_t fs_write(int fd,const void *buf,size_t len);
size_t fs_filesz(int fd);
off_t fs_lseek(int fd,off_t offset,int whence);
int fs_close(int fd);

size_t invalid_read(void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

size_t invalid_write(const void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

/* This is the information about all files in disk. */
static Finfo file_table[] __attribute__((used)) = {
  {"stdin", 0, 0, 0, invalid_read, invalid_write},
  {"stdout", 0, 0, 0, invalid_read, serial_write},
  {"stderr", 0, 0, 0, invalid_read, serial_write},
 [FD_FB]= {"/dev/fb", 0, 0, 0, invalid_read, fb_write},
 [FD_EVENTS]={"dev/events",0,0,0,invalid_read,invalid_write},
// [FD_DISPINFO]={"proc/dispinfo",128,0,0,dispinfo_read,invalid_write},
#include "files.h"
};

#define NR_FILES (sizeof(file_table) / sizeof(file_table[0]))

void init_fs() {
  // TODO: initialize the size of /dev/fb
  file_table[FD_FB].size=screen_height()*screen_width()*sizeof(size_t);
}


int fs_open(const char*pathname,int flags,int mode)
{//Log("I am in open");
	for(int i=0;i<NR_FILES;i++)
	{
	Log("filename:%s",file_table[i].name);
	if(strcmp(file_table[i].name,pathname)==0)
		return i;
	}

	Log("pathname:%s",pathname);
Log("fs_open GG");
assert(0);
return -1;
}

ssize_t fs_read(int fd,void *buf,size_t len)
{//Log("I am in read");
	ssize_t fsize=fs_filesz(fd);
	switch(fd){
		case FD_STDOUT:Log("in case stdout of fs_read");assert(0);
		case FD_STDERR:Log("in case stderr of fs_read");assert(0);
		case FD_FB:Log("in case fd_fb of fs_read");assert(0);
		default:{
					if(file_table[fd].open_offset>fsize)
						return 0;
					if(file_table[fd].open_offset+len>fsize)
				    {
						len=fsize-file_table[fd].open_offset;
					}
				   ramdisk_read(buf,file_table[fd].open_offset+file_table[fd].disk_offset,len);
				}
	}
	file_table[fd].open_offset+=len;
	return len;
}

ssize_t fs_write(int fd,const void *buf,size_t len)
{Log("I am in write");
	ssize_t fsize=fs_filesz(fd);
	switch(fd){
		case FD_STDOUT:Log("in case stdout of fs_write");assert(0);
		case FD_STDERR:Log("in case stderr of fs_write");assert(0);
		case FD_FB:Log("in case fd_fb of fs_write");assert(0);
		default:{
					if(file_table[fd].open_offset>fsize)
						return 0;
					if(file_table[fd].open_offset+len>fsize)
					{
						len=fsize-file_table[fd].open_offset;
					}
					ramdisk_write(buf,file_table[fd].open_offset+file_table[fd].disk_offset,len);	
				}
	}
	file_table[fd].open_offset+=len;
	return len;
}


off_t fs_lseek(int fd,off_t offset,int whence)
{
	switch(whence){
		case SEEK_SET:file_table[fd].open_offset=offset;break;
		case SEEK_CUR:file_table[fd].open_offset+=offset;break;
		case SEEK_END:file_table[fd].open_offset=file_table[fd].size+offset;break;
		default:return -1;
	}
	if(file_table[fd].open_offset>file_table[fd].size)
	{file_table[fd].open_offset=file_table[fd].size;
	}return file_table[fd].open_offset;
}
size_t fs_filesz(int fd)
{
return file_table[fd].size;
}
int fs_close(int fd)
{
return 0;
}

