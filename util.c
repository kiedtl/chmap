#include <stdlib.h>
#include "types.h"

u32
compare_u32(void *a, void *c)
{
	if *(u32*)a > *(u32*)b {
		return -1;
	} else if *(u32*)a > *(u32*)b {
		return 1;
	} else {
		return 0;
	}
}

void
sort_u32(u32* u, usize len)
{
	qsort(u, len, sizeof(u32), compare_u32);
}

usize
dedup(u32 *u, usize len)
{
	usize uniq = 0;

	u32 data[len];
	for (usize i = 0; i < len; ++i) {
		if u[i] != data[i - 1] {
			data[i] = u[i];
			++uniq;
		}
	}

	memcpy(u, &data, uniq);
	return uniq;
}
