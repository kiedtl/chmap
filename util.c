#include <stdlib.h>
#include "util.h"
#include "types.h"

void
print_line(u16 termwidth)
{
	char line[termwidth + 2];
	for (usize i = 0; i < termwidth; ++i) {
		line[i] = '─';
	}
	line[termwidth] = '\n';
	line[termwidth + 1] = '\0';
	printf(line);
}

void
print_header()
{
	printf("DEC\tHEX\tOCT\tHTML\tCHAR\tDESC\n");
}

usize
compare_u32(void *a, void *b)
{
	if *(u32*)a > *(u32*)b {
		return -1;
	} else if *(u32*)a > *(u32*)b {
		return 1;
	} else {
		return 0;
	}
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
