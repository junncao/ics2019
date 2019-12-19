#include "klib.h"

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
    int i = 0;
    for(;s[i];i++){}
    return i;
}

char *strcpy(char* dst,const char* src) {
    int i = 0;
    while(src[i]){
        dst[i] = src[i];
        i++;
    }
    dst[i] = 0;
  return dst;
}

char* strncpy(char* dst, const char* src, size_t n) {
    int i = 0;
    while(src[i] && (i<n)){
        dst[i] = src[i];
        i++;
    }
    while(i<n){
        dst[i] = 0;
        i++;
    }
  return dst;
}

char* strcat(char* dst, const char* src) {
    int i = strlen(dst);
    strcpy((char*)(dst+i), src);
    return dst;
}

int strcmp(const char* s1, const char* s2) {
    int i = 0;
    for(;s1[i];i++){
        if(s1[i]!=s2[i])
            break;
    }
  return (s1[i]-s2[i]);
}

int strncmp(const char* s1, const char* s2, size_t n) {
    int i = 0;
    for(;s1[i] && (i<n);i++){
        if(s1[i]!=s2[i])
            break;
    }
    if(i==n)
        return 0;
    return (s1[i]-s2[i]);
}

void* memset(void* v,int c,size_t n) {
    for(int i = 0; i < n;i++){
        ((int *)v)[i] = c;
    }
    return v;
}

void* memcpy(void* out, const void* in, size_t n) {
    strncpy((char*)out, (char*)in, n);
    return out;
}

int memcmp(const void* s1, const void* s2, size_t n){
    return strncmp((char*)s1, (char*)s2, n);
}

#endif
