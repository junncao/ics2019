#include "cpu/exec.h"

make_EHelper(jal){
    s0 = decinfo.seq_pc;
    s1 = 4;
    rtl_sr(id_dest->reg, &s0, 4);
    rtl_sub(&s0, &s0, &s1);
    rtl_add(&(decinfo.jmp_pc), &s0, &id_src->val);
    //printf("pc:%x\n", decinfo.jmp_pc);
    decinfo_set_jmp(true);
    print_asm_template2(jal);
}

make_EHelper(jalr){
    rtl_sr(id_dest->reg, &s0, 4);
    decinfo.jmp_pc = id_src->val;
    //printf("pc:%x\n", decinfo.jmp_pc);
    decinfo_set_jmp(true);
    print_asm_template2(jalr);
}

make_EHelper(beq){
    decinfo.jmp_pc = decinfo.seq_pc+id_dest->val-4;
    //printf("pc:%x\n", decinfo.jmp_pc);
    decinfo_set_jmp((id_src->val == id_src2->val));
    print_asm_template2(beq);
}
