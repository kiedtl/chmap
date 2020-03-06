#if defined(_WIN32) || defined(__WIN32__)
#define WOE_IS_ME
#endif

/* TODO: sort include alphabetically */
#include "bool.h"
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
	for (usize i = 0; i < (usize) termwidth - 1; ++i) {
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

char
pathsep(void)
{
#ifdef WOE_IS_ME
	return '\\';
#else
	return '/';
#endif
}
