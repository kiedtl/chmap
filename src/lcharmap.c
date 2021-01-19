#include <err.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <string.h>
#include <unistd.h>

#include "argoat.h"
#include "db.h"
#include "dirs.h"
#include "display.c"
#include "util.h"
#include "utf.h"
#include "range.c"

sqlite3 *db;
_Bool istty = false;

static void
range(void *data, char **pars, const int pars_count)
{
	UNUSED(data);

	/*
	 * TODO: support number characters from other languages
	 * e.g. Chinese
	 */
	if (pars_count < 1)
		errx(1, "chmap: error: argument to --range missing.");

	Rune entries[262144];
	ssize_t entries_len = -1;

	if ((entries_len = expand_range(pars[0], entries)) < 0)
		errx(1, "chmap: error: '%s': invalid range.", pars[0]);

	for (size_t i = 0; i < (size_t)entries_len; ++i) {
		char *desc = chardb_getdesc(db, entries[i]);
		printentry(entries[i], desc, istty);
	}
}

static void
chars(void *data, char **pars, const int pars_count)
{
	UNUSED(data);

	if (pars_count < 1)
		errx(1, "chmap: error: argument to --chars missing.");

	/*
	 * we must process each character as a Rune, not byte-wise.
	 * This is to ensure that UTF8 continuation bytes don't get
	 * treated as a separate character.
	 */
	size_t len = utflen(pars[0]);
	Rune *chars = (Rune*) ecalloc(len, sizeof(Rune));
	Rune *p = chars;
	for (size_t i = 0; i < len; ++i, ++p)
		pars[0] += chartorune(p, pars[0]);
	*p = '\0';

	for (size_t i = 0; i < len; ++i) {
		char *desc = chardb_getdesc(db, chars[i]);
		printentry(chars[i], desc, istty);
	}

	if (chars) free(chars);
}

static void
search(void *data, char **pars, const int pars_count)
{
	UNUSED(data);

	if (pars_count < 1)
		errx(1, "chmap: error: argument to --search missing.");

	char *query = pars[0];
	regex_t re;

	/* TODO: get char of error and error message */
	if (regcomp(&re, query, 0))
		errx(1, "chmap: '%s': invalid regex query.", query);

	/*
	 * my excu^Wreason for not allocating on demand: 32kB isn't
	 * *that* much memory.
	 */
	Rune matches[32841];
	size_t match_count = chardb_search(db, &re, (Rune *)&matches);

	if (match_count == 0)
		errx(1, "chmap: error: no results found.");

	for (size_t i = 0; i < match_count; ++i) {
		char *desc = chardb_getdesc(db, matches[i]);
		printentry(matches[i], desc, istty);
	}
}

/* ¯\_(ツ)_/¯ */
static void
handle_anger(void *data, char **pars, const int pars_count)
{
	UNUSED(data);
	UNUSED(pars);
	UNUSED(pars_count);

	errx(1, "rawr");
}

static void
version(void *data, char **pars, const int pars_count)
{
	UNUSED(data);
	UNUSED(pars);
	UNUSED(pars_count);

	errx(0, "chmap v%s\n", VERSION);
}

static void
usage(void *data, char **pars, const int pars_count)
{
	UNUSED(data);
	UNUSED(pars);
	UNUSED(pars_count);

	printf("Usage: chmap [-rcs] [CHAR]... [OPTION]...\n");
	printf("Print and search information on the provided Unicode characters.\n\n");
	printf("OPTIONS:\n");
	printf("    -l, --long          print character entries in the long format.\n");
	printf("    -h, --help          print this help message and exit.\n");
	printf("    -V, --version       print version and exit.\n\n");
	printf("ARGUMENTS:\n");
	printf("    -r, --range RANGE   print a range of Unicode characters.\n");
	printf("    -c, --chars CHARS   print a range of Unicode codepoints that match\n");
	printf("                        provided character(s).\n");
	printf("    -s, --search REGEX  search character descriptions for REGEX.\n\n");
	printf("Report issues to https://github.com/lptstr/chmap.\n");
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
		errx(1, "chmap: can't find character database.");
	db = chardb_open(format("%s%cchmap%cchars.db",
				datadir, pathsep(), pathsep()));

	/* parse arguments with cylgom/argoat */
	const struct argoat_sprig sprigs[15] = {
		/* unflagged */
		{ NULL,      0, NULL,                       NULL         },
		{ "version", 0, NULL,                       version      },
		{ "V",       0, NULL,                       version      },
		/* -v instead of -V will be removed soon */
		{ "v",       0, NULL,                       version      },
		{ "help",    0, NULL,                       usage        },
		{ "h",       0, NULL,                       usage        },
		{ "range",   1, NULL,                       range        },
		{ "r",       1, NULL,                       range        },
		{ "rage",    0, NULL,                       handle_anger },
		{ "chars",   1, NULL,                       chars        },
		{ "c",       1, NULL,                       chars        },
		{ "search",  1, NULL,                       search       },
		{ "s",       1, NULL,                       search       },
	};

	struct argoat args = { sprigs, sizeof(sprigs), NULL, 0, 0 };
	argoat_graze(&args, argc, argv);
}
