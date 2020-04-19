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

#include <stdio.h>

#include "terminfo.h"
#include "types.h"

#if defined(_WIN32) || defined(__WIN32__)
#define WOE_IS_ME
#endif

#ifdef WOE_IS_ME
#else
#include <sys/ioctl.h>
#include <unistd.h>
#endif

/*
 * sizes to fall back to if:
 *     1) stdout isn't a tty
 *     2) if on Windows (temporary, only until win32
 *     support it added
 */
const u16 fallback_width  = 80;
const u16 fallback_height = 24;

/*
 * file descriptor to get dimensions of
 * default: stdout (1)
 */
const usize fd = 1;

u16
ttywidth(void)
{
#ifdef WOE_IS_ME
	return (u16) fallback_width;
#else
	if (!isatty(fd))
		return fallback_width;

	struct winsize w;
	ioctl(fd, TIOCGWINSZ, &w);
	return w.ws_col;
#endif
}

u16
ttyheight(void)
{
#ifdef WOE_IS_ME
	return (u16) 24;
#else
	if (!isatty(fd))
		return fallback_height;

	struct winsize w;
	ioctl(fd, TIOCGWINSZ, &w);

	return w.ws_row;
#endif
}
