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

#if defined(_WIN32) || defined (__WIN32__)
#define WOE_IS_ME
#endif

#ifndef WOE_IS_ME
#include <sys/ioctl.h>
#endif

unsigned short ttywidth(void);
unsigned short ttyheight(void);

unsigned short
ttywidth(void)
{
#ifndef WOE_IS_ME
	struct winsize w;
	ioctl(0, TIOCGWINSZ, &w);

	return w.ws_col;
#else
	return (unsigned short) 80;
#endif
}

unsigned short
ttyheight(void)
{
#ifndef WOE_IS_ME
	struct winsize w;
	ioctl(0, TIOCGWINSZ, &w);

	return w.ws_row;
#else
	return (unsigned short) 24;
#endif
}
