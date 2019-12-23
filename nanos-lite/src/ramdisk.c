#include "common.h"

extern uint8_t ramdisk_start;
extern uint8_t ramdisk_end;
#define RAMDISK_SIZE ((&ramdisk_end) - (&ramdisk_start))

/* The kernel is monolithic, therefore we do not need to
 * translate the address `buf' from the user process to
 * a physical one, which is necessary for a microkernel.
 */

/* read `len' bytes starting from `offset' of ramdisk into `buf' */
size_t ramdisk_read(void *buf, size_t offset, size_t len) {
  assert(offset + len <= RAMDISK_SIZE);
  memcpy(buf, &ramdisk_start + offset, len);
  return len;
}

/* write `len' bytes starting from `buf' into the `offset' of ramdisk */
size_t ramdisk_write(const void *buf, size_t offset, size_t len) {
  assert(offset + len <= RAMDISK_SIZE);
  memcpy(&ramdisk_start + offset, buf, len);
  return len;
}

void init_ramdisk() {
    int size = &ramdisk_end - &ramdisk_start;
    int start = (int)&ramdisk_start;
    int end = (int)&ramdisk_end;
    int mstart = start;
    int mend = end;
    if(size&start&end){
    }
  Log("ramdisk info: start = 0x%x, end = 0x%x",
      mstart, mend);
  //Log("ramdisk info: start = %p, end = %p, size = %d bytes",
   //   &ramdisk_start, &ramdisk_end, size);
}

size_t get_ramdisk_size() {
  return RAMDISK_SIZE;
}
