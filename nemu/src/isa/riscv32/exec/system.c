#include "cpu/exec.h"

//static int32_t spec, sstatus, sscause, stvec;

int32_t readcsr(int i){
    switch(i){
        case 0:
        default:
            assert(0 && "Unfinished readcsr");
    }
}

void writecsr(int i, int32_t val){
    //TODO
}

make_EHelper(system){
    s0 = id_src->val;
    switch(decinfo.isa.instr.funct3){
        // csrrw
        case 0b001:
 //           break;
        default:
            assert(0 && "Unfinished system op");
    }
}

