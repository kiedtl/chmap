#ifndef UTIL_H
#define UTIL_H
#include "types.h"

usize die(const char *fmt, ...);
void  print_line(u16 termwidth);
void  print_header(void);
char  pathsep(void);

#endif
