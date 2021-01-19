#if defined(_WIN32) || defined(__WIN32__)
#define WOE_IS_ME
#endif

#include <assert.h>
#include <err.h>
#include <execinfo.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <string.h>

#include "db.h"
#include "util.h"

extern sqlite3 *db;

void *
ecalloc(size_t ct, size_t sz)
{
	void *mem = calloc(ct, sz);
	if (mem == NULL)
		err(1, "lcharmap: can't allocate memory:");
	return mem;
}

_Noreturn void __attribute__((format(printf, 1, 2)))
die(const char *fmt, ...)
{
	fprintf(stderr, "fatal: ");

	va_list ap;
	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);

	if (fmt[0] && fmt[strlen(fmt) - 1] == ':') {
		perror(" ");
	} else {
		fputc('\n', stderr);
	}

	char *buf_sz_str = getenv("CHMAP_DEBUG");

	if (buf_sz_str == NULL) {
		fprintf(stderr, "NOTE: set $CHMAP_DEBUG >0 for a backtrace.\n");
	} else {
		size_t buf_sz = strtol(buf_sz_str, NULL, 10);
		void *buffer[buf_sz];

		int nptrs = backtrace(buffer, buf_sz);
		char **strings = backtrace_symbols(buffer, nptrs);
		assert(strings);

		fprintf(stderr, "backtrace:\n");
		for (size_t i = 0; i < (size_t) nptrs; ++i)
			fprintf(stderr, "   %s\n", strings[i]);
		free(strings);
	}

	_Exit(EXIT_FAILURE);
}

char * __attribute__((format(printf, 1, 2)))
format(const char *fmt, ...)
{
	static char buf[8192];
	memset(buf, 0x0, sizeof(buf));
	va_list ap;
	va_start(ap, fmt);
	int len = vsnprintf(buf, sizeof(buf), fmt, ap);
	va_end(ap);
	assert((size_t) len < sizeof(buf));
	return (char *) &buf;
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
