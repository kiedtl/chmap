/*
 * dirs.c: provides functionality similar to Rust's dirs crate.
 * (c) Kiëd Llaentenn
 * See the LICENSE.md file for license information.
 *
 * TODO: separate this into it's own submodule and maintain separately
 * TODO: provide the rest of the functionality of the `dirs` crate
 *
 * Please note that this file will ONLY support Linux and Windows.
 * no support for macOS, BSD, Minix, Haiku, Redox, etc, etc, etc
 *
 * See also: https://docs.rs/dirs/2.0.2/dirs/
 */

#if defined(_WIN32) || defined(__WIN32__)
#define WOE_IS_ME
#endif

#ifdef WOE_IS_ME
#else
#include <pwd.h>
#include <sys/types.h>
#include <unistd.h>
#endif

#include <string.h>
#include <stdlib.h>
#include "dirs.h"
#include "types.h"

char*
dirs_data_dir(void)
{
#ifdef WOE_IS_ME
	/* TODO: Windows support */
	return NULL;
#else
	char *data_dir = getenv("XDG_DATA_HOME");
	if (data_dir == NULL) {
		char *home = dirs_home_dir();

		usize size = strlen(home)
			+ (sizeof("/.local/share") - 1)
			+ 1 /* null terminator */;
		char *buf = malloc(size * sizeof(char));
		buf[size - 1] = '\0';

		strcpy(buf, home);
		strcat(buf, "/.local/share");

		return buf;
	}

	return data_dir;
#endif
}

char*
dirs_home_dir(void)
{
	char *HOME = getenv("HOME");
	if (HOME == NULL) {
#ifdef WOE_IS_ME
		/* TODO: Windows support */
		return NULL;
#else
		/* read /etc/passwd */
		struct passwd *pwdata = getpwuid(getuid());
		return pwdata->pw_dir;
#endif
	}

	return HOME;
}
