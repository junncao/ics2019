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

make_EHelper(bra){
    decinfo.jmp_pc = decinfo.seq_pc+id_dest->val-4;
    switch(decinfo.isa.instr.funct3){
        case 0:
            decinfo_set_jmp((id_src->val == id_src2->val));
            print_asm_template2(beq);
            break;
        case 1:
            decinfo_set_jmp((id_src->val != id_src2->val));
            print_asm_template2(bne);
            break;
        case 4:
            decinfo_set_jmp(((signed)id_src->val < (signed)id_src2->val));
            print_asm_template2(blt);
            break;
        case 5:
            decinfo_set_jmp((id_src->val >= id_src2->val));
            print_asm_template2(bge);
            break;
        default:
            assert(0 && "Unfinished branch opcode");

    }
}
