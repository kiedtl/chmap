#ifndef DB_H
#define DB_H

#include <stdint.h>
#include <regex.h>
#include <sqlite3.h>

sqlite3 *chardb_open(char *path);
void    chardb_close(sqlite3 *db);
char    *chardb_getdesc(sqlite3 *db, uint32_t _char);
size_t  chardb_search(sqlite3 *db, regex_t *re, uint32_t *matchbuf);

#endif
