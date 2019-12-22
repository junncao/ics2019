#include "cpu/exec.h"

//static int32_t spec, sstatus, sscause, stvec;
static int32_t stvec;

int32_t readcsr(int i){
    switch(i){
        case 0x105:
            return stvec;
        default:
            assert(0 && "Unfinished readcsr");
    }
}

void writecsr(int i, int32_t val){
    //TODO
    switch(i){
        case 0x105:
            stvec = val;
            break;
        default:
            assert(0 && "Unfinished readcsr");
    }
}

make_EHelper(system){
    Instr instr = decinfo.isa.instr;
    s0 = readcsr(instr.csr);
    switch(instr.funct3){
        // csrrw
        case 0b001:
            writecsr(instr.csr, s0);
            rtl_sr(id_dest->reg, &s0, 4);
            break;
        default:
            assert(0 && "Unfinished system op");
    }
}

