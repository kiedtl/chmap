#ifndef UTIL_H
#define UTIL_H

#define SQL(...)  #__VA_ARGS__
#define UNUSED(V) ((void)(V))

void* ecalloc(size_t ct, size_t sz);
_Noreturn void __attribute__((format(printf, 1, 2))) die(const char *fmt, ...);
char * __attribute__((format(printf, 1, 2))) format(const char *fmt, ...);
char  pathsep(void);

#endif
