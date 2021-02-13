#include <err.h>
#include <regex.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <string.h>
#include <unistd.h>
#include <utf8proc.h>

#include "arg.h"
#include "db.h"
#include "dirs.h"
#include "display.c"
#include "util.h"
#include "range.c"

sqlite3 *db;
_Bool istty = false;
_Bool flong = false;

static void
range(char *param)
{
	/*
	 * TODO: support number characters from other languages
	 * e.g. Chinese
	 */

	uint32_t entries[262144];
	ssize_t entries_len = -1;

	if ((entries_len = expand_range(param, entries)) < 0)
		errx(1, "'%s': invalid range.", param);

	printheader(flong);
	for (size_t i = 0; i < (size_t)entries_len; ++i) {
		char *desc = chardb_getdesc(db, entries[i]);
		printentry(entries[i], desc, istty, flong);
	}
}

static void
chars(char *param)
{
	unsigned char *inp = (unsigned char *)param;

	int32_t charbuf = 0;
	ssize_t runelen = 0;

	printheader(flong);

	while (*inp) {
		charbuf = 0;
		runelen = utf8proc_iterate(inp, -1, &charbuf);

		if (runelen < 0) {
			size_t offset = (char *)inp - param;
			warnx("invalid byte %zu: %s",
				offset, utf8proc_errmsg(runelen));
			++inp;
			continue;
		}

		inp += runelen;

		char *desc = chardb_getdesc(db, charbuf);
		printentry(charbuf, desc, istty, flong);
	}
}

static void
search(char *param)
{
	char *query = param;
	regex_t re;

	/* TODO: get char of error and error message */
	if (regcomp(&re, query, 0))
		errx(1, "'%s': invalid regex query.", query);

	/*
	 * my excu^Wreason for not allocating on demand: 32kB isn't
	 * *that* much memory.
	 */
	uint32_t matches[32841];
	size_t match_count = chardb_search(db, &re, (uint32_t *)&matches);

	if (match_count == 0)
		errx(1, "no results found.");

	printheader(flong);

	for (size_t i = 0; i < match_count; ++i) {
		char *desc = chardb_getdesc(db, matches[i]);
		printentry(matches[i], desc, istty, flong);
	}
}

static void
usage(_Bool _short)
{
	printf("Usage: chmap [-l] -r RANGE\n");
	printf("       chmap [-l] -c CHARS\n");
	printf("       chmap [-l] -s REGEX\n");

	if (_short)
		exit(0);

	printf("\n");
	printf("Print information for Unicode characters.\n");
	printf("\n");
	printf("OPTIONS:\n");
	printf("    -l, --long          print character entries in the long format.\n");
	printf("    -h, --help          print this help message and exit.\n");
	printf("    -V, --version       print version and exit.\n");
	printf("\n");
	printf("FLAGS:\n");
	printf("    -r, --range RANGE   print a range of Unicode characters.\n");
	printf("    -c, --chars CHARS   print a range of Unicode codepoints that match\n");
	printf("                        provided character(s).\n");
	printf("    -s, --search REGEX  search character descriptions for REGEX.\n");
	printf("\n");
	printf("Full documentation is available locally at chmap(1).\n");

	exit(0);
}

int
main(int argc, char **argv)
{
	istty = isatty(STDOUT_FILENO);

	/* load database */
	char *datadir = dirs_data_dir();
	if (datadir == NULL)
		errx(1, "can't find character database.");
	db = chardb_open(format("%s%cchmap%cchars.db",
				datadir, pathsep(), pathsep()));

	ARGBEGIN {
	break; case 'l':
		flong = !flong;
	break; case 'r':
		range(EARGF(usage(true)));
	break; case 'c':
		chars(EARGF(usage(true)));
	break; case 's':
		search(EARGF(usage(true)));
	break; case 'V': case 'v':
		printf("chmap v%s\n", VERSION);
		return 0;
	break; case 'h':
		usage(false);
	break; default:
		usage(true);
	} ARGEND
}
