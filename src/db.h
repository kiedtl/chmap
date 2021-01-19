#ifndef DB_H
#define DB_H

#include <regex.h>
#include <sqlite3.h>
#include "utf.h"

sqlite3 *chardb_open(char *path);
void    chardb_close(sqlite3 *db);
char    *chardb_getdesc(sqlite3 *db, Rune _char);
size_t  chardb_search(sqlite3 *db, regex_t *re, Rune *matchbuf);

#endif
