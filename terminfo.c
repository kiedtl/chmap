/*
 * terminfo.c: get TTY/console's buffer
 * height and width.
 *
 * TODO: make this work on Windows.
 * TODO: test this on Windows 10.
 *
 * why is all Windows programming so
 * damn complicated.
 *
 * (c) Kiëd Llaentenn and contributors
 * See the LICENSE.md file for copyright
 * information.
 */

#include "terminfo.h"
#include "types.h"

#if defined(_WIN32) || defined(__WIN32__)
#define WOE_IS_ME
#endif

#ifdef WOE_IS_ME
#else
#include <sys/ioctl.h>
#endif

u16
ttywidth(void)
{
#ifdef WOE_IS_ME
	return (u16) 80;
#else
	struct winsize w;
	ioctl(0, TIOCGWINSZ, &w);

	return w.ws_col;
#endif
}

u16
ttyheight(void)
{
#ifdef WOE_IS_ME
	return (u16) 24;
#else
	struct winsize w;
	ioctl(0, TIOCGWINSZ, &w);

	return w.ws_row;
#endif
}
