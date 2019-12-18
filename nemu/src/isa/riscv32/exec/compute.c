#include "cpu/exec.h"

make_EHelper(lui) {
  rtl_sr(id_dest->reg, &id_src->val, 4);

  print_asm_template2(lui);
}

make_EHelper(addi) {
  rtl_add(&id_dest->val, &id_src->val, &id_src2->val);
  rtl_sr(id_dest->reg, &id_dest->val, 4);

  print_asm_template2(addi);
}

make_EHelper(calu) {
  switch(decinfo.isa.instr.funct3){
      case 0:
          if(decinfo.isa.instr.funct7){
            rtl_sub(&id_dest->val, &id_src->val, &id_src2->val);
            print_asm_template2(sub);
          }
          else{
            rtl_add(&id_dest->val, &id_src->val, &id_src2->val);
            print_asm_template2(add);
          }
          break;
          //TODO: remaining thing may need to fill
      case 1:
            //rtl_sll(&id_dest->val, &id_src->val, &id_src2->val);
            print_asm_template2(sll);
            break;
      case 2:
            //rtl_slt(&id_dest->val, &id_src->val, &id_src2->val);
            print_asm_template2(slt);
            break;
      case 3:
            //rtl_sltu(&id_dest->val, &id_src->val, &id_src2->val);
            print_asm_template2(slt);
            break;

  }
  rtl_sr(id_dest->reg, &id_dest->val, 4);

}

make_EHelper(auipc){
    s0 = decinfo.seq_pc;
    rtl_add(&id_src->val, &s0, &id_src->val);
    rtl_sr(id_dest->reg, &id_src->val, 4);
    print_asm_template2(auipc);
}
