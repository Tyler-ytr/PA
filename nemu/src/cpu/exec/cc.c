#include "cpu/rtl.h"

/* Condition Code */

void rtl_setcc(rtlreg_t* dest, uint8_t subcode) {
  bool invert = subcode & 0x1;
  enum {
    CC_O, CC_NO, CC_B,  CC_NB,
    CC_E, CC_NE, CC_BE, CC_NBE,
    CC_S, CC_NS, CC_P,  CC_NP,
    CC_L, CC_NL, CC_LE, CC_NLE
  };

  // TODO: Query EFLAGS to determine whether the condition code is satisfied.
  // dest <- ( cc is satisfied ? 1 : 0)
  switch (subcode & 0xe) {
    case CC_O://Log("I am cc 0");assert(0);
			*dest=cpu.eflags.OF;break;
    case CC_B://Log("I am cc B");assert(0);
			*dest=cpu.eflags.CF;
			break;
	case CC_E://printf("ZF: %d\n",cpu.eflags.ZF);
			  *dest=cpu.eflags.ZF;break;
    case CC_BE://Log("I am cc_be");
			   *dest=((cpu.eflags.CF)||(cpu.eflags.ZF));
			   break;
    case CC_S://Log("I am cc s");assert(0);
			   *dest=cpu.eflags.SF;
			   break;
    case CC_L://Log("I am cc l");
			  *dest=(cpu.eflags.SF!=cpu.eflags.OF);break;
    case CC_LE://Log("I am cc le");
			  // Log("ZF: %d SF: %d OF: %d",cpu.eflags.ZF,cpu.eflags.SF,cpu.eflags.OF);
	*dest=((cpu.eflags.ZF)||(cpu.eflags.SF!=cpu.eflags.OF));break;

    //  TODO();
    default: panic("should not reach here");
    case CC_P: panic("n86 does not have PF");
  }

  if (invert) {
    rtl_xori(dest, dest, 0x1);
  }
}
