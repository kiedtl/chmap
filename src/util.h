#ifndef UTIL_H
#define UTIL_H

#define UNUSED(V) ((void)(V))

char * __attribute__((format(printf, 1, 2))) format(const char *fmt, ...);

#endif
