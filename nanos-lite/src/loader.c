#include "proc.h"
#include "fs.h"
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
  for(int i = 0; i < Ehdr.e_phnum;i++){
      Elf_Phdr Phdr;
      ramdisk_read(&Phdr, Ehdr.e_phoff + i*Ehdr.e_phentsize, sizeof(Phdr));
      if(!(Phdr.p_type & PT_LOAD)){
          continue;
      }
      ramdisk_read((void*)Phdr.p_vaddr, Phdr.p_offset, Phdr.p_filesz);
  }

  Elf_Phdr Phdr;
  ramdisk_read(&Phdr, Ehdr.e_phoff + 0*Ehdr.e_phentsize, sizeof(Phdr));
  int fd = fs_open(filename, 0, 0);
  fs_lseek(fd, 0, 0);
  printf("%x\n", Phdr.p_filesz);
  int res = fs_read(fd, (void*)Phdr.p_vaddr, 0x5657);
  printf("%x\n", res);
  return Ehdr.e_entry;
}

void naive_uload(PCB *pcb, const char *filename) {
  intptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %x", entry);
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
