#include <am.h>
#include <x86.h>
#include <amdev.h>
#include <klib.h>

static uint32_t* const fb __attribute__((used)) = (uint32_t *)0x40000;

size_t video_read(uintptr_t reg, void *buf, size_t size) {
  switch (reg) {
    case _DEVREG_VIDEO_INFO: {
      _VideoInfoReg *info = (_VideoInfoReg *)buf;
      info->width = inw(0x100);
     // info->width =400;
      info->height =( inl(0x100)>>16);
     // info->height =300;
      return sizeof(_VideoInfoReg);
    }
  }
  return 0;
}

size_t video_write(uintptr_t reg, void *buf, size_t size) {
  switch (reg) {
    case _DEVREG_VIDEO_FBCTL: {
      _FBCtlReg *ctl = (_FBCtlReg *)buf;
	  
	//  int size=screen_width()*screen_height();
	  int x=ctl->x,y=ctl->y,w=ctl->w,h=ctl->h;
	  uint32_t *pixels=ctl->pixels;
	  int k;
	  if(w<400-x)
		  k=w;
	  else k=400-x;
	  int cp_bytes=sizeof(uint32_t)*k;
	  for(int j=0;j<h&&y+j<300;j++)
	  {
		  memcpy(&fb[(y+j)*400+x],pixels,cp_bytes);
		  
		  pixels+=w;
	  
	  
	  
	  }
      if (ctl->sync) {
        // do nothing, hardware syncs.
      }
      return sizeof(_FBCtlReg);
    }
  }
  return 0;
}

void vga_init() {
}
