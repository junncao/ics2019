#include "common.h"
#include <amdev.h>
#include <am.h>
#include <nemu.h>

size_t __am_input_read(uintptr_t reg, void *buf, size_t size);
size_t __am_timer_read(uintptr_t reg, void *buf, size_t size);

size_t serial_write(const void *buf, size_t offset, size_t len) {
    for(int i = 0; i < len;i++){
        _putc(((char*)buf)[i]);
    }
  return len;
}

#define NAME(key) \
  [_KEY_##key] = #key,

static const char *keyname[256] __attribute__((used)) = {
  [_KEY_NONE] = "NONE",
  _KEYS(NAME)
};

size_t events_read(void *buf, size_t offset, size_t len) {
    int kc = read_key();
    char tmp[3]="ku";
    if((kc & 0xfff) == _KEY_NONE){
        int time = uptime();
        len = sprintf(buf,"t %d\n", time);
        printf("%s", buf);
    }
    else{
        if(kc&0x8000)
            tmp[1] = 'd';
        len = sprintf(buf,"%s %s\n",tmp,keyname[kc&0xfff]);
        printf("%s", buf);
    }
  return len;
}

static char dispinfo[128] __attribute__((used)) = {};

size_t dispinfo_read(void *buf, size_t offset, size_t len) {
  return 0;
}

size_t fb_write(const void *buf, size_t offset, size_t len) {
  return 0;
}

size_t fbsync_write(const void *buf, size_t offset, size_t len) {
  return 0;
}

void init_device() {
  Log("Initializing devices...");
  _ioe_init();

  // TODO: print the string to array `dispinfo` with the format
  // described in the Navy-apps convention
}
