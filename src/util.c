#if defined(_WIN32) || defined(__WIN32__)
#define WOE_IS_ME
#endif

/* TODO: sort include alphabetically */
#include <stdarg.h>
#include "bool.h"
#include <stdio.h>
#include <string.h>
#include <sqlite3.h>
#include <stdlib.h>
#include "util.h"
#include "db.h"
#include "lcharmap.h"
#include "types.h"

extern sqlite3 *db;
extern struct Options *opts;

void*
ecalloc(usize ct, usize sz)
{
	void *mem = calloc(ct, sz);
	if (mem == NULL)
		die("lcharmap: error: unable to allocate memory:");
	return mem;
}

usize
die(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);

	if (fmt[0] && fmt[strlen(fmt)-1] == ':') {
		fputc(' ', stderr);
		perror(NULL);
	} else {
		fputc('\n', stderr);
	}

	cleanup();
	exit(1);

	/* return dummy value so that die() can
	 * be used with a boolean expression
	 * e.g.: is_ok || die("message")
	 */
	return 0;
}

void
cleanup(void)
{
	chardb_close(db);
	free(opts);
}

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