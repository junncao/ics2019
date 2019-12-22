#include "cpu/exec.h"

extern void raise_intr(uint32_t, vaddr_t);
int32_t readcsr(int i){
    switch(i){
        case 0x105:
            return decinfo.isa.stvec;
        default:
            assert(0 && "Unfinished readcsr");
    }
}

void writecsr(int i, int32_t val){
    //TODO
    switch(i){
        case 0x105:
            decinfo.isa.stvec = val;
            break;
        default:
            assert(0 && "Unfinished readcsr");
    }
}

make_EHelper(system){
    Instr instr = decinfo.isa.instr;
    switch(instr.funct3){
        //ecall
        case 0b0:
            raise_intr(0, cpu.pc);
            break;
        // csrrw
        case 0b001:
            s0 = readcsr(instr.csr);
            writecsr(instr.csr, id_src->val);
            rtl_sr(id_dest->reg, &s0, 4);
            break;
        default:
            printf("%d", instr.funct3);
            assert(0 && "Unfinished system op");
    }
}

