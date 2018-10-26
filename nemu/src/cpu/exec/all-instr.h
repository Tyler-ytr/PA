#include "cpu/exec.h"

make_EHelper(mov);
make_EHelper(movzx);

make_EHelper(push);
make_EHelper(pop);

make_EHelper(sub);
make_EHelper(add);
make_EHelper(idiv);
make_EHelper(div);
make_EHelper(adc);
make_EHelper(dec);
make_EHelper(inc);

make_EHelper(and);
make_EHelper(or);
make_EHelper(xor);
make_EHelper(sar);
make_EHelper(shl);
make_EHelper(imul1);
make_EHelper(imul2);
make_EHelper(imul3);

make_EHelper(cltd);

make_EHelper(call);
make_EHelper(jmp);
make_EHelper(leave);
make_EHelper(test);
make_EHelper(not);
make_EHelper(operand_size);
make_EHelper(lea);
make_EHelper(cmp);

make_EHelper(jcc);
make_EHelper(ret);
make_EHelper(inv);
make_EHelper(nemu_trap);

make_EHelper(nop);
make_EHelper(setcc);
