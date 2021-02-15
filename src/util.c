#include <assert.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "util.h"
#include "unicode.h"

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
