#include "cpu/exec.h"
#include "memory/mmu.h"

void raise_intr(uint8_t NO, vaddr_t ret_addr) {
  /* TODO: Trigger an interrupt/exception with ``NO''.
   * That is, use ``NO'' to index the IDT.
   */

 //TODO();
	

	rtl_push((rtlreg_t *)&cpu.eflags);
	rtl_push((rtlreg_t *)&cpu.cs);
	rtl_push((rtlreg_t *)&ret_addr);

	uint32_t idtr_base=cpu.idtr.base;
//	uint32_t eip_low,eip_high,offset;
	uint32_t address=NO*8+idtr_base;

	uint32_t offset=vaddr_read(address+4,4);
	uint32_t selector=vaddr_read(address,4);
	offset=(offset&0xffff0000)+(selector&0xffff);
	
	rtl_j(offset);



//	eip_low=vaddr_read(idtr_base+NO*8,4)&0x0000ffff;
//	eip_high=vaddr_read(idtr_base+NO*8+4,4)&0xffff0000;
//	offset=eip_low|eip_high;
//	decoding.jmp_eip=offset;
//	rtl_j(offset);
//	decoding.jmp_eip=offset;
//	decoding.is_jmp=true;








 
}

void dev_raise_intr() {
}
