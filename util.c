/* TODO: sort include alphabetically */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "util.h"
#include "types.h"

void
print_line(u16 termwidth)
{
	/* TODO: allow user to change line character
	 * via cmd options, just in case their terminal/font
	 * doesn't fully support UTF8 */

	/* don't forget the null terminator! */
	char line[((termwidth) * sizeof("─")) + 1];
	strcpy(line, "─");
	for (usize i = 0; i < termwidth - 1; ++i) {
		strcat(line, "─");
	}
	line[termwidth * sizeof("─")] = '\n';
	line[(termwidth + 1) * sizeof("─")] = '\0';
	printf(line);
}

void
print_header(void)
{
	printf("DEC\tHEX\tOCT\tHTML\tCHAR\tDESC\n");
}

int
compare_char(void *a, void *b)
{
	if (*(char*)a > *(char*)b) {
		return -1;
	} else if (*(char*)a > *(char*)b) {
		return 1;
	} else {
		return 0;
	}
}

int
compare_u32(void *a, void *b)
{
	if (*(u32*)a > *(u32*)b) {
		return -1;
	} else if (*(u32*)a > *(u32*)b) {
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
		if (u[i] != data[i - 1]) {
			data[i] = u[i];
			++uniq;
		}
	}

	memcpy(u, &data, uniq);
	return uniq;
}
