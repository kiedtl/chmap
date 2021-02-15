#ifndef UTF8_H
#define UTF8_H

#include <stdint.h>

ssize_t utf8_char_length(char c);
ssize_t utf8_char_to_unicode(uint32_t *out, const char *c);
size_t  utf8_unicode_to_char(char *out, uint32_t c);

#endif
