#if defined(_WIN32) || defined(__WIN32__)
#define WOE_IS_ME
#endif

#include <assert.h>
#include <stdint.h>
#include <err.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"
#include "unicode.h"

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
	struct CharInfo ci = charinfos[c];
	return ci.lower != ci.upper && ci.upper == -1 && ci.category != UC_Lt;
}

_Bool
utf8islower(uint32_t c)
{
	struct CharInfo ci = charinfos[c];
	return ci.lower != ci.upper && ci.lower == -1;
}
