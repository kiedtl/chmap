#ifndef LCHARMAP_H
#define LCHARMAP_H

#include "bool.h"
#include "types.h"
#include "utf.h"

/* VERSION defined in makefile */
const char  ESCAPE = 0x1B;
const usize LONG_FORMAT_PADDING = 20;

struct Options {
	bool format_long;
	u16  ttywidth;
};

sqlite3 *db;
struct Options *opts;

void range(void *data, char **pars, const int pars_count);
void chars(void *data, char **pars, const int pars_count);
void search(void *data, char **pars, const int pars_count);

void print_entry_short(Rune entry, char *description);
void print_entry_long(Rune entry, char *description);
void print_entry_row(char *key, char *val);

void handle_bool(void *data, char **pars, const int pars_count);
void handle_anger(void *data, char **pars, const int pars_count);
void version(void *data, char **pars, const int pars_count);
void usage(void *data, char **pars, const int pars_count);

#endif
