/*
 * dirs.c: provides functionality similar to Rust's dirs crate.
 * (c) Kiëd Llaentenn
 * See the LICENSE.md file for license information.
 *
 * TODO: separate this into it's own submodule and maintain separately
 * TODO: provide the rest of the functionality of the `dirs` crate
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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "dirs.h"

char *
dirs_data_dir(void)
{
#ifdef WOE_IS_ME
	/* TODO: Windows support */
	return NULL;
#else
	char *data_dir = getenv("XDG_DATA_HOME");
	if (data_dir == NULL) {
		char *home = dirs_home_dir();

		static char buf[4096];
		memset(buf, 0x0, sizeof(buf));
		sprintf(buf, "%s/.local/share", home);
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
		struct passwd *pwdata = getpwuid(getuid());
		return pwdata->pw_dir;
#endif
	}

	return HOME;
}
