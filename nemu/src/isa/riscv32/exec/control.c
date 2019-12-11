#include "cpu/exec.h"

make_EHelper(auipc){
    rtl_sr(id_dest->reg, &id_src->val, 4);
    decinfo_set_jmp(1);
    decinfo.jmp_pc = decinfo.seq_pc + id_src->val;
    print_asm_template2(auipc);
}
