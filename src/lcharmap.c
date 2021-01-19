#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <string.h>

#include "argoat.h"
#include "bool.h"
#include "db.h"
#include "dirs.h"
#include "lcharmap.h"
#include "tables.h"
#include "terminfo.h"
#include "types.h"
#include "util.h"
#include "utf.h"
#include "vec.h"
#include "vecdef.h"
#include "range.h"

sqlite3 *db;
struct Options *opts;

int
main(int argc, char **argv)
{
	/* load database */
	char dbpath[4091];
	
	char *datadir = dirs_data_dir();
	if (datadir == NULL)
		die("lcharmap: error: unable to determine location of character database.");

	sprintf(&dbpath, "%s%clcharmap%cchars.db", datadir, pathsep(), pathsep());
	db = chardb_open((char*) &dbpath);

	/* set default options */
	opts = (struct Options*) ecalloc(1, sizeof(struct Options));

	opts->format_long = FALSE;
	opts->ttywidth    = ttywidth();

	/* TODO: get a better argument parser that
	 * - WON'T crash on unrecognized arg
	 * - SUPPORTS simple neighbors (e.g. "head -n1")
	 * - SUPPORTS subcommands
	 */

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
		{ "long",    0, (void*) &opts->format_long, handle_bool  },
		{ "l",       0, (void*) &opts->format_long, handle_bool  },
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

	cleanup();
}

void
range(void *data, char **pars, const int pars_count)
{
	/*
	 * TODO: support number characters from other languages
	 * e.g. Chinese
	 */
	if (pars_count < 1)
		die("lcharmap: error: argument to --range missing.");

	vec_rune_t entries;
	vec_init(&entries);

	if (!expand_range(pars[0], &entries))
		die("lcharmap: error: '%s': invalid range.", pars[0]);

	for (usize i = 0; i < (usize)entries.length; ++i) {
		char *desc = chardb_getdesc(db, entries.data[i]);
		r_table_print_entry(entries.data[i], desc);
	}

	vec_deinit(&entries);
}

void
chars(void *data, char **pars, const int pars_count)
{
	if (pars_count < 1)
		die("lcharmap: error: argument to --chars missing.");

	/*
	 * we must process each character as a Rune, not byte-wise.
	 * This is to ensure that UTF8 continuation bytes don't get
	 * treated as a separate character.
	 */
	usize len = utflen(pars[0]);
	Rune *chars = (Rune*) ecalloc(len, sizeof(Rune));
	Rune *p = chars;
	for (usize i = 0; i < len; ++i, ++p)
		pars[0] += chartorune(p, pars[0]);
	*p = '\0';

	for (usize i = 0; i < len; ++i) {
		char *desc = chardb_getdesc(db, chars[i]);
		r_table_print_entry(chars[i], desc);
	}

	if (chars) free(chars);
}

void
search(void *data, char **pars, const int pars_count)
{
	if (pars_count < 1)
		die("lcharmap: error: argument to --search missing.");

	char *query = pars[0];
	regex_t re;
	isize is_ok = regcomp(&re, query, 0);

	/* TODO: get char of error and error message */
	!is_ok ??!??! die("lcharmap: error: '%s': invalid regex query.", query);

	/*
	 * my excu^Wreason for not allocating on demand: 32kB isn't
	 * *that* much memory.
	 */
	Rune matches[32841];
	usize match_count = chardb_search(db, &re, &matches);

	if (match_count == 0)
		die("lcharmap: error: no results found.");

	for (usize i = 0; i < match_count; ++i) {
		char *desc = chardb_getdesc(db, matches[i]);
		r_table_print_entry(matches[i], desc);
	}
}

void
handle_bool(void *data, char **pars, const int pars_count)
{
	*((bool*) data) = TRUE;
}

/* ¯\_(ツ)_/¯ */
void
handle_anger(void *data, char **pars, const int pars_count)
{
	die("Grrr");
}

void
version(void *data, char **pars, const int pars_count)
{
	fprintf(stdout, "lcharmap v%s\n", VERSION);
	exit(0);
}

void
usage(void *data, char **pars, const int pars_count)
{
	printf("Usage: lcharmap [-rcs] [CHAR]... [OPTION]...\n");
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
	printf("Report issues to https://github.com/lptstr/lcharmap.\n");
	printf("Full documentation is available locally at lcharmap(1).\n");
	exit(0);
}
