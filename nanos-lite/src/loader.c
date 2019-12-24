#include "proc.h"
#include <elf.h>

extern size_t ramdisk_read(void *, size_t, size_t);
extern size_t ramdisk_write(const void*, size_t, size_t);
extern void isa_vaddr_write(uint32_t, uint32_t, int);

#ifdef __ISA_AM_NATIVE__
# define Elf_Ehdr Elf64_Ehdr
# define Elf_Phdr Elf64_Phdr
#else
# define Elf_Ehdr Elf32_Ehdr
# define Elf_Phdr Elf32_Phdr
#endif

static uintptr_t loader(PCB *pcb, const char *filename) {
  Elf_Ehdr Ehdr;
  ramdisk_read(&Ehdr, 0, sizeof(Ehdr));
  int maddr = 0;
  for(int i = 0;i < Ehdr.e_phnum;i++){
      Elf_Phdr Phdr;
      ramdisk_read(&Phdr, Ehdr.e_phoff + i*Ehdr.e_phentsize, sizeof(Phdr));
      if(Phdr.p_type & PT_LOAD){
          continue;
      }
      //char a;
      //unsigned int j = 0;
      if(i==0){
        maddr = Phdr.p_vaddr;
      }
      ramdisk_read((void*)Phdr.p_vaddr, Phdr.p_offset, Phdr.p_filesz);
      for(int j = 0; j < 10;j++){
          printf("0x%x ", *((int*)(Phdr.p_vaddr+i*4)));
      }
      /*
      for(; j < Phdr.p_filesz; j++){
          ramdisk_read(&a, Phdr.p_offset + j, sizeof(char));
          isa_vaddr_write(Phdr.p_vaddr + j, a, sizeof(char));
      }
      */
      /*
      for(; j < Phdr.p_memsz;j++){
          isa_vaddr_write(Phdr.p_vaddr + j, 0, sizeof(char));
      }
      */
  }
  return maddr;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %x", entry);
  return;
  ((void(*)())entry) ();
}

void context_kload(PCB *pcb, void *entry) {
  _Area stack;
  stack.start = pcb->stack;
  stack.end = stack.start + sizeof(pcb->stack);

  pcb->cp = _kcontext(stack, entry, NULL);
}

void context_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);

  _Area stack;
  stack.start = pcb->stack;
  stack.end = stack.start + sizeof(pcb->stack);

  pcb->cp = _ucontext(&pcb->as, stack, stack, (void *)entry, NULL);
}
