#include "cpu/exec.h"
#include "cpu/cc.h"

make_EHelper(jmp) {
  // the target address is calculated at the decode stage
  rtl_j(decoding.jmp_eip);

  print_asm("jmp %x", decoding.jmp_eip);
}

make_EHelper(jcc) {
  // the target address is calculated at the decode stage
//Log("I am in jcc!");
  	uint32_t cc = decoding.opcode & 0xf;
  rtl_setcc(&t0, cc);
  rtl_li(&t1, 0);
  rtl_jrelop(RELOP_NE, &t0, &t1, decoding.jmp_eip);

  print_asm("j%s %x", get_cc_name(cc), decoding.jmp_eip);
}

make_EHelper(jmp_rm) {
  rtl_jr(&id_dest->val);

  print_asm("jmp *%s", id_dest->str);
}

make_EHelper(call) {
  // the target address is calculated at the decode stage
 // TODO();
//printf("callmiaomiaomiao\n");
rtl_push(&decoding.seq_eip);
//rtl_addi(&decoding.jmp_eip,eip,id_dest->val);

//Log("Call from 0x%x to 0x%x\n",cpu.eip,decoding.jmp_eip);
rtl_j(decoding.jmp_eip);
//assert(0);
  print_asm("call %x", decoding.jmp_eip);
}

make_EHelper(ret) {
//  TODO();
rtl_pop(&decoding.jmp_eip);

//decoding.is_jmp=1;
//Log("ret into  0x%x\n",decoding.jmp_eip);

rtl_j(decoding.jmp_eip);
  print_asm("ret");
}

make_EHelper(call_rm) {
//  TODO();

rtl_push(&decoding.seq_eip);
rtl_j(id_dest->val);
  print_asm("call *%s", id_dest->str);
}
