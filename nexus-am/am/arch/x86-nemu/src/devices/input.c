#include <am.h>
#include <x86.h>
#include <amdev.h>
//I8042_DATA_PORT=0X60
size_t input_read(uintptr_t reg, void *buf, size_t size) {
  switch (reg) {
    case _DEVREG_INPUT_KBD: {
      _KbdReg *kbd = (_KbdReg *)buf;
     // kbd->keydown = 0;
     // kbd->keycode = _KEY_NONE;
		if(kbd->keydown==1)
			kbd->keycode=inl(0x60);
		else
      kbd->keycode = _KEY_NONE;
			
      return sizeof(_KbdReg);
    }
  }
  return 0;
}
