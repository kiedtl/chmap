#if defined(_WIN32) || defined(__WIN32__)
#define WOE_IS_ME
#endif

#include <assert.h>
#include <err.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <string.h>
#include <utf8proc.h>

#include "db.h"
#include "util.h"

extern sqlite3 *db;

void *
ecalloc(size_t ct, size_t sz)
{
	void *mem = calloc(ct, sz);
	if (mem == NULL)
		err(1, "chmap: can't allocate memory:");
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

_Bool
utf8isupper(uint32_t c)
{
	const utf8proc_property_t *p = utf8proc_get_property((int32_t)c);
	return p->lowercase_seqindex != p->uppercase_seqindex
		&& p->uppercase_seqindex == UINT16_MAX
		&& p->category != UTF8PROC_CATEGORY_LT;
}

_Bool
utf8islower(uint32_t c)
{
	const utf8proc_property_t *p = utf8proc_get_property((int32_t)c);
	return p->lowercase_seqindex != p->uppercase_seqindex
		&& p->lowercase_seqindex == UINT16_MAX;
}
