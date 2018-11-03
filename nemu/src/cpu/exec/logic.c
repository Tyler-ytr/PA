#include "cpu/exec.h"
#include "cpu/cc.h"

make_EHelper(test) {
//  TODO();
//	Log("in test\n");
	rtl_and(&t2,&id_dest->val,&id_src->val);
	
	rtl_update_ZFSF(&t2,id_dest->width);
	t0=0;
	rtl_set_CF(&t0);
	rtl_set_OF(&t0);



  print_asm_template2(test);
}

make_EHelper(and) {
//  TODO();
//	Log("id_dest: %x id_src=%x id_dest->width=%x id_src->width=%x",id_dest->val,id_src->val,id_dest->width,id_src->width);
	rtl_sext(&t0,&id_src->val,id_src->width);	
	rtl_and(&t2,&id_dest->val,&t0);
	operand_write(id_dest,&t2);
//	Log(" t2: %x\n",t2);
	rtl_update_ZFSF(&t2,id_dest->width);
	t0=0;	
	rtl_set_CF(&t0);
	rtl_set_OF(&t0);

	print_asm_template2(and);
}
make_EHelper(rol)
{
	rtl_shl(&t1,&id_dest->val,&id_src->val);//11010 shl 3 10000 shr 2 00110 rol 3 10110
	rtl_shri(&t0,&id_dest->val,id_src->width*8-id_src->val);
	rtl_or(&t2,&t0,&t1);
	operand_write(id_dest,&t2);


//CF,OF may be wrong wait for set

  print_asm_template2(rol);
}
make_EHelper(xor) {
//  TODO();
	rtl_xor(&t2,&id_dest->val,&id_src->val);
	operand_write(id_dest,&t2);
	rtl_update_ZFSF(&t2,id_dest->width);
	t0=0;
	rtl_set_CF(&t0);
	rtl_set_OF(&t0);

  print_asm_template2(xor);
}

make_EHelper(or) {
//  TODO();
//Log("I am in or");

	rtl_or(&t2,&id_dest->val,&id_src->val);
	operand_write(id_dest,&t2);
	rtl_update_ZFSF(&t2,id_dest->width);
	t0=0;
	rtl_set_CF(&t0);
	rtl_set_OF(&t0);
  print_asm_template2(or);
}

make_EHelper(sar) {
//  TODO();
  // unnecessary to update CF and OF in NEMU
	if(id_dest->width==1)
	{
		id_dest->val=(int8_t)id_dest->val;
	}else if(id_dest->width==2)
	{
		id_dest->val=(int16_t)id_dest->val;
	}


	rtl_sar(&t2,&id_dest->val,&id_src->val);
	operand_write(id_dest,&t2);
	rtl_update_ZFSF(&t2,id_dest->width);
	
  print_asm_template2(sar);
}

make_EHelper(shl) {
//  TODO();
  // unnecessary to update CF and OF in NEMU
	
	rtl_shl(&t2,&id_dest->val,&id_src->val);
	operand_write(id_dest,&t2);
	rtl_update_ZFSF(&t2,id_dest->width);
  print_asm_template2(shl);
}

make_EHelper(shr) {
//  TODO();
  // unnecessary to update CF and OF in NEMU
	
	rtl_shr(&t2,&id_dest->val,&id_src->val);
	operand_write(id_dest,&t2);
	rtl_update_ZFSF(&t2,id_dest->width);
  print_asm_template2(shr);
}

make_EHelper(setcc) {
  uint32_t cc = decoding.opcode & 0xf;

  rtl_setcc(&t2, cc);
  operand_write(id_dest, &t2);

  print_asm("set%s %s", get_cc_name(cc), id_dest->str);
}

make_EHelper(not) {
//  TODO();

	rtl_not(&t2,&id_dest->val);
	operand_write(id_dest,&t2);
  print_asm_template1(not);
}
