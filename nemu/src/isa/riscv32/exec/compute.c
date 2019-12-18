#include "cpu/exec.h"

make_EHelper(lui) {
  rtl_sr(id_dest->reg, &id_src->val, 4);

  print_asm_template2(lui);
}

make_EHelper(calui) {
   switch(decinfo.isa.instr.funct3){
      case 0:
            rtl_addi(&id_dest->val, &id_src->val, decinfo.isa.instr.simm11_0);
            print_asm_template2(addi);
          break;
      case 2:
            id_dest->val = (signed)id_src->val < (signed)decinfo.isa.instr.simm11_0;
            print_asm_template2(slti);
            break;
      case 3:
            id_dest->val = (unsigned)id_src->val < (unsigned)decinfo.isa.instr.simm11_0;
            print_asm_template2(sltiu);
            break;
      case 4:
            rtl_xori(&id_dest->val, &id_src->val, decinfo.isa.instr.simm11_0);
            print_asm_template2(xori);
            break;
      case 5:
          if(decinfo.isa.instr.funct7){
            rtl_sari(&id_dest->val, &id_src->val, decinfo.isa.instr.rs2);
            print_asm_template2(sari);
          }
          else{
            rtl_shri(&id_dest->val, &id_src->val, decinfo.isa.instr.rs2);
            print_asm_template2(shri);
          }
          break;
      case 7:
            rtl_andi(&id_dest->val, &id_src->val, decinfo.isa.instr.simm11_0);
            print_asm_template2(andi);
            break;
      default:
            assert(0 && "Unfinished CALUI opcode");

  }
  rtl_sr(id_dest->reg, &id_dest->val, 4);
}

make_EHelper(calu) {
  int funct7 = (unsigned)decinfo.isa.instr.funct7;
  switch(decinfo.isa.instr.funct3){
      case 0:
          if(funct7 & 0b0100000){
            rtl_sub(&id_dest->val, &id_src->val, &id_src2->val);
            print_asm_template2(sub);
          }
          else if(funct7 & 0b1){
            rtl_mul_lo(&id_dest->val, &id_src->val, &id_src2->val);
            print_asm_template2(mul);
          }
          else{
            rtl_add(&id_dest->val, &id_src->val, &id_src2->val);
            print_asm_template2(add);
          }
          break;
          //TODO: remaining thing may need to fill
      case 1:
            rtl_shl(&id_dest->val, &id_src->val, &id_src2->val);
            print_asm_template2(sll);
            break;
      case 2:
            id_dest->val = (signed)id_src->val < (signed)id_src2->val;
            print_asm_template2(slt);
            break;
      case 3:
            id_dest->val = (unsigned)id_src->val < (unsigned)id_src2->val;
            print_asm_template2(sltu);
            break;
      case 4:
          if(funct7 & 0b1){
            rtl_div_q(&id_dest->val, &id_src->val, &id_src2->val);
            print_asm_template2(div);
          }
          else{
            rtl_xor(&id_dest->val, &id_src->val, &id_src2->val);
            print_asm_template2(xor);
          }
            break;
      case 5:
          if(decinfo.isa.instr.funct7){
            rtl_sar(&id_dest->val, &id_src->val, &id_src2->val);
            print_asm_template2(sar);
          }
          else{
            rtl_shr(&id_dest->val, &id_src->val, &id_src2->val);
            print_asm_template2(shr);
          }
            break;
      case 6:
          if(decinfo.isa.instr.funct7){
            rtl_div_r(&id_dest->val, &id_src->val, &id_src2->val);
            print_asm_template2(rem);
          }
          else{
            rtl_or(&id_dest->val, &id_src->val, &id_src2->val);
            print_asm_template2(or);
          }
            break;
      case 7:
            rtl_and(&id_dest->val, &id_src->val, &id_src2->val);
            print_asm_template2(and);
            break;
      default:
            assert(0 && "Unfinished CALU opcode");

  }
  rtl_sr(id_dest->reg, &id_dest->val, 4);

}

make_EHelper(auipc){
    s0 = decinfo.seq_pc;
    rtl_add(&id_src->val, &s0, &id_src->val);
    rtl_sr(id_dest->reg, &id_src->val, 4);
    print_asm_template2(auipc);
}

