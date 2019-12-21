#include "nemu.h"
#include "monitor/diff-test.h"

bool isa_difftest_checkregs(CPU_state *ref_r, vaddr_t pc) {
    /*
    CPU_state test;
    ref_difftest_getregs((void*)&test);
    for(int i = 0; i < 32;i++){
        if(test.gpr[i]._32!=ref_r->gpr[i]._32){
            return false;
        }
    }
    if(test.pc != pc)
        return false;
        */
    for(int i = 0; i < 32;i++){
        if(reg_l(i)!=ref_r->gpr[i]._32){
            return false;
        }
    }
    //if(ref_r->pc != pc)
     //   return false;

    return true;
}

void isa_difftest_attach(void) {
}
