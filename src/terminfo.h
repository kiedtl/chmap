/*
 * terminfo.h: get TTY/console's buffer
 * height and width.
 *
 * (c) Kiëd Llaentenn and contributors
 * See the LICENSE.md file for copyright
 * information.
 */

#ifndef TERMINFO_H
#define TERMINFO_H
#include "types.h"

u16 ttywidth(void);
u16 ttyheight(void);

#endif
