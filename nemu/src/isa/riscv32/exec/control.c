#include "cpu/exec.h"
make_EHelper(jal){
    s0 = decinfo.seq_pc;
    rtl_sr(id_dest->reg, &id_src->val, 4);
    decinfo_set_jmp(true);
    decinfo.jmp_pc = id_src->val;
}
