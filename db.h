#ifndef DB_H
#define DB_H

#include <regex.h>
#include <sqlite3.h>
#include "types.h"

sqlite3 *chardb_open(char *path);
void    chardb_close(sqlite3 *db);
char    *chardb_getdesc(sqlite3 *db, u32 _char);
usize   chardb_search(sqlite3 *db, regex_t *re, u32 *matchbuf);

#endif
