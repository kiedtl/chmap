#include "bool.h"
#include "db.h"
#include "types.h"
#include "utf.h"

#include <alloca.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include <unistd.h>

/* TODO: make database loading/descriptions optional */
/* TODO: allow user to modify db path at runtime via cmd args */
sqlite3*
chardb_open(char *path)
{
	sqlite3 *db;

	/* just in case the user didn't install
	 * the character database */
	if (access(path, R_OK) != -1) {
		int rc = sqlite3_open(path, &db);
		if (rc) {
			fprintf(stderr,
				"lcharmap: error: unable to access character database: %s\n",
				sqlite3_errmsg(db));
			sqlite3_close(db);
			exit(1);
		}

		return db;
	} else {
		fprintf(stderr,
			"lcharmap: error: unable to open character database at '%s'.\n",
			path);
		exit(1);
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
	usize querylen = snprintf(NULL, 0, "SELECT description FROM map WHERE id=%i;", _char);
	char query[querylen];

	char *err = NULL;
	sprintf((char*) &query, "SELECT description FROM map WHERE id=%i;", _char);
	sqlite3_stmt *stmt;
	sqlite3_prepare_v2(db, (char*) &query, querylen, &stmt, NULL);
	sqlite3_step(stmt);

	/* copy string onto our buffer, to prevent sqlite3_finalize from ruining it */
	char *desc;
	if (sqlite3_column_text(stmt, 0) != NULL) {
		desc = strdup((char*) sqlite3_column_text(stmt, 0));
	} else {
		desc = "";
	}

	sqlite3_finalize(stmt);

	if (err != NULL) {
		fprintf(stderr, "lcharmap: warning: %s", err);
	}

	return desc;
}

usize
chardb_search(sqlite3 *db, regex_t *re, Rune *matchbuf)
{
	usize mctr = 0;

	/* TODO: panic on sqlite err */
	char *query = "SELECT description FROM map;";
	usize query_len = strlen(query);

	sqlite3_stmt *stmt;
	sqlite3_prepare_v2(db, query, query_len, &stmt, NULL);

	isize result = 0;
	for (usize i = 0; ; ++i) {
		result = sqlite3_step(stmt);

		/* is another row available? */
		if (result != SQLITE_ROW) {
			break;
		}

		char *desc = (char*) sqlite3_column_text(stmt, 0);
		bool match = !regexec(re, desc, 0, NULL, 0);
		
		if (match) {
			matchbuf[mctr] = i;
			++mctr;
		}
	}

	regfree(re);
	return mctr;
}
