#ifndef UTIL_H
#define UTIL_H
#include "types.h"

void* ecalloc(usize ct, usize sz);
_Noreturn void __attribute__((format(printf, 1, 2))) die(const char *fmt, ...);
char * __attribute__((format(printf, 1, 2))) format(const char *fmt, ...);
void  cleanup(void);
char  pathsep(void);

#endif
