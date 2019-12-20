#include "klib.h"
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

int printf(const char *fmt, ...) {
  return 0;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
    int count = 0, flag = 0;
    char tmpc[30], sflag;
    char *tmp;
    int inte;
    int i = 0, j = 0, k;

    for(;fmt[i];i++){
        if(flag){
            flag = 0;
            switch(fmt[i]){
                case 's':
                    tmp = va_arg(ap,char*);
                    strcat(out+j, tmp);
                    j += strlen(tmp);
                    count++;
                    break;
                case 'd':
                    inte = va_arg(ap, int);
                    k = 0;
                    sflag = inte < 0;
                    inte = inte *(sflag?-1:1);
                    if(sflag){
                        out[j] = '-';
                        j++;
                    }
                    do{
                        tmpc[k] = inte % 10;
                        inte /= 10;
                        k++;
                    }while(inte);
                    assert(tmpc[0] != 1);
                    for(int l = 0; l < k;l++){
                        out[j+l] = tmpc[k-1-l] + '0';
                    }
                    assert(out[j] != '1');
                    j+=k;
                    count++;
                    break;
                    //TODO: deal with '%'
                default:
                    assert(0 && "Unfinish stdio");
            }
            continue;
        }
        if(fmt[i] == '%'){
            flag = 1;
            continue;
        }
        out[j] = fmt[i];
        j++;
    }
    va_end(ap);
    return count;
}

int sprintf(char *out, const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    return vsprintf(out, fmt, ap);
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  return 0;
}

#endif
