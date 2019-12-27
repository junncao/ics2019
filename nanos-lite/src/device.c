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
    _DEV_INPUT_KBD_t kbd;
    __am_input_read(_DEVREG_INPUT_KBD, &kbd, 0);
    char tmp[3]="kd";
    if(kbd.keycode == _KEY_NONE){
        _DEV_TIMER_UPTIME_t timer;
        __am_timer_read(_DEVREG_TIMER_UPTIME, &timer, 0);
        sprintf(buf+offset,"t %d\n", timer.lo);
        printf("%s", buf+offset);
    }
    else{
        if(!kbd.keydown)
            tmp[1] = 'u';
        sprintf(buf+offset,"%s %s\n",tmp,keyname[kbd.keycode]);
    }
  return 0;
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
