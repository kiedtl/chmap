#include <regex.h>
#include <stdio.h>
#include <string.h>
#include <sqlite3.h>
#include <unistd.h>

#include "types.h"

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
chardb_getdesc(sqlite3 *db, u32 _char)
{
	usize max_query_size = (sizeof("SELECT description FROM map WHERE id=") - 1)
		+ (sizeof("32841") - 1)
		+ (sizeof(";") - 1);
	char query[max_query_size];

	char *err = NULL;
	sprintf(&query, "SELECT description FROM map WHERE id=%i;", _char);
	sqlite3_exec(db, &query, NULL, NULL, &err);

	if (err != NULL) {
		fprintf(stderr, "lcharmap: warning: character database error: %s\n", err);
	}

	return desc;
}

usize
chardb_search(sqlite3 *db, regex_t *re, u32 *matchbuf)
{
	usize mctr = 0;

	/* TODO: panic on sqlite err */
	char *query = "SELECT description FROM map;";
	usize query_len = strlen(query);

	sqlite3_stmt stmt;
	sqlite3_prepare_v2(db, query, query_len, &stmt, NULL);

	isize result = 0;
	for (usize i = 0; ; ++i) {
		result = sqlite3_step(stmt);

		/* is another row available? */
		if result != SQLITE_ROW {
			break;
		}

		char *desc = sqlite3_column_test(stmt, 0);
		bool match = regexec(re, desc, 0, NULL, 0);
		
		if (match) {
			matchbuf[mctr] = strtol(sqlite3_column_text(stmt, 0), 10);
			++mctr;
		}
	}

	regfree(re);
	return mctr;
}
