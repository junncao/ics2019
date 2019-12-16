#include "cpu/exec.h"
make_EHelper(jal){
    s0 = decinfo.seq_pc;
    rtl_sr(id_dest->reg, &s0, 4);
    rtl_add(&(decinfo.jmp_pc), &s0, &id_src->val);
    decinfo_set_jmp(true);
}
