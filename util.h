#ifndef UTIL_H
#define UTIL_H
#include "types.h"

void  print_line(u16 termwidth);
void  print_header(void);
int   compare_u32(void *a, void *b);
usize dedup(u32 *u, usize len);

#endif
