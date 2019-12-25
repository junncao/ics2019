#include "fs.h"
extern size_t ramdisk_read(void*, size_t, size_t);
extern size_t ramdisk_write(const void*, size_t, size_t);
typedef size_t (*ReadFn) (void *buf, size_t offset, size_t len);
typedef size_t (*WriteFn) (const void *buf, size_t offset, size_t len);

typedef struct {
  char *name;
  size_t size;
  size_t disk_offset;
  size_t open_offset;
  ReadFn read;
  WriteFn write;
} Finfo;

enum {FD_STDIN, FD_STDOUT, FD_STDERR, FD_FB};

size_t invalid_read(void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

size_t invalid_write(const void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

/* This is the information about all files in disk. */
static Finfo file_table[] __attribute__((used)) = {
  {"stdin", 0, 0, 0, invalid_read, invalid_write},
  {"stdout", 0, 0, 0, invalid_read, invalid_write},
  {"stderr", 0, 0, 0, invalid_read, invalid_write},
#include "files.h"
};

#define NR_FILES (sizeof(file_table) / sizeof(file_table[0]))

int fs_open(const char *pathname, int flags, int mode){
    for(int i = 3; i < NR_FILES;i++){
        if(strcmp(pathname, file_table[i].name) == 0){
            return i;
        }
    }
    assert(0 && "Can't find file");
}

size_t fs_read(int fd, void *buf, size_t len){
    if(file_table[fd].open_offset+len > file_table[fd].size){
        if(file_table[fd].size > file_table[fd].open_offset)
            len = file_table[fd].size - file_table[fd].open_offset;
        else
            len = 0;
    }
    ramdisk_read(buf, file_table[fd].disk_offset + file_table[fd].open_offset, len);
    file_table[fd].open_offset += len;
    return len;
}

size_t fs_write(int fd, const void *buf, size_t len){
    if(file_table[fd].open_offset+len > file_table[fd].size){
        if(file_table[fd].size > file_table[fd].open_offset)
            len = file_table[fd].size - file_table[fd].open_offset;
        else
            len = 0;
    }
    ramdisk_write(buf, file_table[fd].disk_offset + file_table[fd].open_offset, len);
    file_table[fd].open_offset += len;
    return len;
}

size_t fs_lseek(int fd, size_t offset, int whence){
    //TODO: change offset according to whence
    file_table[fd].open_offset = offset;
    return offset;
}

int fs_close(int fd){
    return 0;
}

void init_fs() {
  // TODO: initialize the size of /dev/fb
}
