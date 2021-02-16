#ifndef UTF8_H
#define UTF8_H

#include <stdint.h>

uint8_t utf8_bytesz(char c);
ssize_t utf8_decode(uint32_t *out, char *c, size_t sz);
ssize_t utf8_encode(char *out, uint32_t c);

#endif
