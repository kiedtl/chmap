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

#include "argoat.h"
#include "db.h"
#include "dirs.h"
#include "display.c"
#include "util.h"
#include "range.c"

sqlite3 *db;
_Bool istty = false;
_Bool flong = false;

static void
set_bool(void *d, char **_, const int _c)
{
	UNUSED(_), UNUSED(_c);
	*(_Bool*)d = !*(_Bool*)d;
}

static void
range(void *data, char **pars, const int pars_count)
{
	UNUSED(data);

	/*
	 * TODO: support number characters from other languages
	 * e.g. Chinese
	 */
	if (pars_count < 1)
		errx(1, "argument to --range missing.");

	uint32_t entries[262144];
	ssize_t entries_len = -1;

	if ((entries_len = expand_range(pars[0], entries)) < 0)
		errx(1, "'%s': invalid range.", pars[0]);

	for (size_t i = 0; i < (size_t)entries_len; ++i) {
		char *desc = chardb_getdesc(db, entries[i]);
		printentry(entries[i], desc, istty, flong);
	}
}

static void
chars(void *data, char **pars, const int pars_count)
{
	UNUSED(data);

	if (pars_count < 1)
		errx(1, "argument to --chars missing.");

	unsigned char *inp = (unsigned char *)pars[0];

	int32_t charbuf = 0;
	ssize_t runelen = 0;

	while (*inp) {
		charbuf = 0;
		runelen = utf8proc_iterate(inp, -1, &charbuf);

		if (runelen < 0) {
			size_t offset = (char *)inp - pars[0];
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
search(void *data, char **pars, const int pars_count)
{
	UNUSED(data);

	if (pars_count < 1)
		errx(1, "argument to --search missing.");

	char *query = pars[0];
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

	for (size_t i = 0; i < match_count; ++i) {
		char *desc = chardb_getdesc(db, matches[i]);
		printentry(matches[i], desc, istty, flong);
	}
}

/* ¯\_(ツ)_/¯ */
static void
anger(void *data, char **pars, const int pars_count)
{
	UNUSED(data), UNUSED(pars), UNUSED(pars_count);

	fprintf(stderr, "rawr");
	exit(0);
}

static void
version(void *data, char **pars, const int pars_count)
{
	UNUSED(data);
	UNUSED(pars);
	UNUSED(pars_count);

	printf("chmap v%s\n", VERSION);
	exit(0);
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
		errx(1, "can't find character database.");
	db = chardb_open(format("%s%cchmap%cchars.db",
				datadir, pathsep(), pathsep()));

	/* parse arguments with cylgom/argoat */
	const struct argoat_sprig sprigs[15] = {
		/* unflagged */
		{ NULL,      0, NULL,                       NULL     },
		/* TODO: remove -v */
		{ "v",       0, NULL,                       version  },
		{ "V",       0, NULL,                       version  },
		{ "version", 0, NULL,                       version  },
		{ "h",       0, NULL,                       usage    },
		{ "help",    0, NULL,                       usage    },
		{ "r",       1, NULL,                       range    },
		{ "range",   1, NULL,                       range    },
		{ "rage",    0, NULL,                       anger    },
		{ "c",       1, NULL,                       chars    },
		{ "chars",   1, NULL,                       chars    },
		{ "s",       1, NULL,                       search   },
		{ "search",  1, NULL,                       search   },
		{ "l",       0, &flong,                     set_bool },
		{ "long",    0, &flong,                     set_bool },
	};

	struct argoat args = { sprigs, sizeof(sprigs), NULL, 0, 0 };
	argoat_graze(&args, argc, argv);
}
