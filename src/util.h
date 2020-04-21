#ifndef UTIL_H
#define UTIL_H
#include "types.h"

void* ecalloc(usize ct, usize sz);
usize die(const char *fmt, ...);
void  cleanup(void);
void  print_line(u16 termwidth);
void  print_header(void);
char  pathsep(void);

#endif
