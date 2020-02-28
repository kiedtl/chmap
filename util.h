#ifndef UTIL_H
#define UTIL_H

#include "types.h"

u32 compare_u32(void *a, void *b);
void sort_u32(u32 *u, usize len);
usize dedup(u32 *u, usize len);

#endif
