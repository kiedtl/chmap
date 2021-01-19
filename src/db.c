#include <alloca.h>
#include <err.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include <unistd.h>

#include "util.h"
#include "db.h"
#include "utf.h"

/* TODO: make database loading/descriptions optional */
/* TODO: allow user to modify db path at runtime via cmd args */
sqlite3*
chardb_open(char *path)
{
	sqlite3 *db;

	/* just in case the user didn't install
	 * the character database */
	if (access(path, R_OK) != -1) {
		if (sqlite3_open(path, &db)) {
			warnx("chmap: can't open character db: %s",
					sqlite3_errmsg(db));
			sqlite3_close(db);
			exit(1);
		}

		return db;
	} else {
		errx(1, "chmap: can't access character db at '%s'.\n", path);
	}
}

void
chardb_close(sqlite3 *db)
{
	sqlite3_close(db);
}

char*
chardb_getdesc(sqlite3 *db, Rune _char)
{
	char *query = SQL(
		SELECT description FROM map WHERE id=:id;
	);

	char *err = NULL;
	sqlite3_stmt *stmt;
	sqlite3_prepare_v2(db, query, strlen(query), &stmt, NULL);
	ssize_t index = sqlite3_bind_parameter_index(stmt, ":id");
	if (!index || sqlite3_bind_int64(stmt, index, (sqlite3_int64)_char))
		die("sqlite3 error: %s", sqlite3_errmsg(db));
	sqlite3_step(stmt);

	/* copy string onto our buffer, to prevent sqlite3_finalize from ruining it */
	char *desc;
	if (sqlite3_column_text(stmt, 0) != NULL) {
		desc = strdup((char*) sqlite3_column_text(stmt, 0));
	} else {
		desc = "";
	}

	sqlite3_finalize(stmt);

	if (err != NULL)
		die("sqlite3 error: %s", err);

	return desc;
}

size_t
chardb_search(sqlite3 *db, regex_t *re, Rune *matchbuf)
{
	size_t mctr = 0;

	/* TODO: panic on sqlite err */
	char *query = "SELECT description FROM map;";
	size_t query_len = strlen(query);

	sqlite3_stmt *stmt;
	sqlite3_prepare_v2(db, query, query_len, &stmt, NULL);

	ssize_t result = 0;
	for (size_t i = 0; ; ++i) {
		result = sqlite3_step(stmt);

		/* is another row available? */
		if (result != SQLITE_ROW) {
			break;
		}

		char *desc = (char*) sqlite3_column_text(stmt, 0);
		_Bool match = !regexec(re, desc, 0, NULL, 0);
		
		if (match) {
			matchbuf[mctr] = i;
			++mctr;
		}
	}

	regfree(re);
	return mctr;
}
