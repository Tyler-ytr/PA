#include "common.h"
#include <amdev.h>

size_t serial_write(const void *buf, size_t offset, size_t len) {
//  return 0;
	int i=0;
	for(;i<len;i++)
	{
		_putc(((char*)buf)[i]);
	}
	return len;
}

#define NAME(key) \
  [_KEY_##key] = #key,

static const char *keyname[256] __attribute__((used)) = {
  [_KEY_NONE] = "NONE",
  _KEYS(NAME)
};

size_t events_read(void *buf, size_t offset, size_t len) {
  return 0;
}

static char dispinfo[128] __attribute__((used));

size_t dispinfo_read(void *buf, size_t offset, size_t len) {
	 memcpy(buf, (char*)dispinfo + offset, len);
   	return len;
}

size_t fb_write(const void *buf, size_t offset, size_t len) {
	int my_x,my_y,my_len;
	offset=offset/4;
	my_x=offset%screen_width();	
	my_y=offset/screen_width();
	my_len=len/4;
	Log("offset:%d len:%d x:%d y:%d",offset,my_len,my_x,my_y);
	draw_rect((uint32_t*)buf,my_x,my_y,my_len,1);
  	return len;
}

void init_device() {
  Log("Initializing devices...");
  _ioe_init();
//	sprintf(dispinfo, "WIDTH : %d\nHEIGHT:%d\n", _screen.width, _screen.height);
  // TODO: print the string to array `dispinfo` with the format
  // described in the Navy-apps convention
	sprintf(dispinfo, "WIDTH : %d\nHEIGHT:%d", screen_width(), screen_height());
}
