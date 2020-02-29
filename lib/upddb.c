/*
 * upddb: update lcharmap character database (chars.db)
 * (c) Kiëd Llaentenn
 * see the LICENSE for more information.
*/

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>
#include "types.h"

int
main(void)
{
	FILE *unicode_txt;
	if ((unicode_txt = fopen("./unicode.txt", "r")) == NULL) {
		fprintf(stderr, "error: can't open ./unicode.txt.\n");
		return 1;
	}

	sqlite3 *db;
	int rc = sqlite3_open("./chars.db", &db);
	if (rc) {
		fprintf(stderr, "error: can't open ./chars.db for writing.\n");
		return 1;
	}

	/*
	 * we're assuming that the makefile has already
	 * purged the file
	 */
	char *query = "CREATE TABLE map (id int, description varchar(255));";
	usize query_len = strlen(query);
	sqlite3_stmt *stmt;
	sqlite3_prepare_v2(db, query, query_len, &stmt, NULL);
	sqlite3_step(stmt);
	sqlite3_finalize(stmt);

	/* wrap inserts in transaction to improve insert speed */
	char *err = NULL;
	sqlite3_exec(db, "PRAGMA synchronous = OFF", NULL, NULL, &err);
	sqlite3_exec(db, "PRAGMA journal_mode = MEMORY", NULL, NULL, &err);
	sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, &err);

	/* pre-compile sql queries */
	usize insertq_len = (sizeof("INSERT INTO map VALUES (") - 1)
			+ (sizeof("32841") - 1) /* 32841 = num of chars */
			+ (sizeof(", '") - 1)
			+ 90 + (sizeof("');") - 1);
	char insertq[insertq_len];
	sprintf((char*) &insertq, "INSERT INTO map VALUES ($ID, '$DE');");
	sqlite3_stmt *insert_stmt;
	sqlite3_prepare_v2(db, insertq, insertq_len, &insert_stmt, NULL);

	char *line = NULL;
	usize len  = 0;
	isize read = 0;
	usize linectr = 0;
	while ((read = getline(&line, &len, unicode_txt)) != -1) {
		/* TODO: don't hardcode maximum number of entries */
		fprintf(stdout, "\rstoring description for character %i/%i", linectr, 32841);

		char *tmp = NULL;
		char *data[20];
		usize i = 0;

		/* save pointer to line so that we can free it later */
		char *lineptr = line;
		while (++i, (tmp = strsep(&lineptr, ";"))) {
			data[i] = tmp;
		}

		char *desc = data[1];

		/*
		 * the "descriptions" for some characters aren't really
		 * descriptions, just '<control>'
		 * TODO: get proper descriptions for other control chars (like 0x81)
		 */
		if (linectr < 32) {
			desc = data[10];
		}

		/*
		 * descriptions in unicode.txt are uppercase
		 * and as we all know, uppercase is ugly
		 */
		usize desclen = strlen(desc);
		for (i = 0; i < desclen; ++i)
			desc[i] = tolower(desc[i]);

		char id[snprintf(NULL, 0, "%d", linectr)];
		sprintf((char*) &id, "%d", linectr);

		sqlite3_bind_text(insert_stmt, 1, (char*) &id, -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(insert_stmt, 2, desc, -1, SQLITE_TRANSIENT);
		sqlite3_step(insert_stmt);
		sqlite3_clear_bindings(insert_stmt);
		sqlite3_reset(insert_stmt);
		++linectr;
	}

	fprintf(stdout, "\n");
	sqlite3_finalize(insert_stmt);
	sqlite3_exec(db, "END TRANSACTION", NULL, NULL, &err);
	sqlite3_close(db);
	free(line);
	fclose(unicode_txt);
}
