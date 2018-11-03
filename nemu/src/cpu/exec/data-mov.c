#include "cpu/exec.h"

make_EHelper(mov) {
  operand_write(id_dest, &id_src->val);
  print_asm_template2(mov);
}

make_EHelper(push) {
//assert(0);
 // 	TODO();
//Log("in push"); 	
rtl_sext(&id_dest->val,&id_dest->val,id_dest->width);
// dest <- signext(src1[(width * 8 - 1) .. 0]) 
rtl_push(&id_dest->val);
//Log("push 0x%x into 0x%x\n",id_dest->val,cpu.esp);
  print_asm_template1(push);
}

make_EHelper(pop) {
//  TODO();
//printf("into pop\n");
rtl_pop(&t2);
operand_write(id_dest,&t2);
//Log("pop 0x%x",t2);
  print_asm_template1(pop);
}

make_EHelper(pusha) {
  TODO();

  print_asm("pusha");
}

make_EHelper(popa) {
  TODO();

  print_asm("popa");
}

make_EHelper(leave) {
//  TODO();
	reg_l(R_ESP)=reg_l(R_EBP);
	rtl_lm(&t0,&reg_l(R_ESP),id_src->width);
	id_dest->type=OP_TYPE_REG;
	id_dest->reg=R_EBP;
	operand_write(id_dest,&t0);
	if(id_dest->width==2)
	{
	reg_w(R_ESP)+=2;
	
	
	}else
	{
	reg_l(R_ESP)+=4;
	
	
	}


  print_asm("leave");
}

make_EHelper(cltd) {
  if (decoding.is_operand_size_16) {
 // TODO();
 rtl_lr(&t1,R_AX,2);
	if((int32_t)(int16_t)(uint16_t)t1<0)
		{t2=0;t0=0;
			rtl_addi(&t0,&t2,0xffff);
			rtl_sr(R_DX,&t2,2);
 		}
	else
 		{t2=0;
		rtl_sr(R_DX,&t2,2);
 		}	
  }
   else {
  //  TODO();
	rtl_lr(&t1,R_EAX,4);
	if((int32_t)t1<0)
		{t2=0;t0=0;
		rtl_addi(&t0,&t2,0xffffffff);
		rtl_sr(R_EDX,&t2,4);	
		}
	else
		{
		t2=0;
		rtl_sr(R_EDX,&t2,4);	
		}
   }

  print_asm(decoding.is_operand_size_16 ? "cwtl" : "cltd");
}

make_EHelper(cwtl) {
  if (decoding.is_operand_size_16) {
   // TODO();
   rtl_lr(&t1,R_AL,1);
t1=(int16_t)(int8_t)(uint8_t)t1;
rtl_sr(R_AX,&t1,2);
  }
  else {
   // TODO();
   rtl_lr(&t1,R_AX,2);
   t1=(int32_t)(int16_t)(uint16_t)t1;
   rtl_sr(R_EAX,&t1,4);

  }

  print_asm(decoding.is_operand_size_16 ? "cbtw" : "cwtl");
}

make_EHelper(movsx) {
  id_dest->width = decoding.is_operand_size_16 ? 2 : 4;
  rtl_sext(&t0, &id_src->val, id_src->width);
  operand_write(id_dest, &t0);
  print_asm_template2(movsx);
}

make_EHelper(movzx) {
  id_dest->width = decoding.is_operand_size_16 ? 2 : 4;
  operand_write(id_dest, &id_src->val);
  print_asm_template2(movzx);
}

make_EHelper(lea) {
  operand_write(id_dest, &id_src->addr);
  print_asm_template2(lea);
}
