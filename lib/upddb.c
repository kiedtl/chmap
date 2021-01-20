/*
 * upddb: update lcharmap character database (chars.db)
 * (c) Kiëd Llaentenn
 * see the LICENSE for more information.
*/

#include <ctype.h>
#include <err.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

#define SQL(...) #__VA_ARGS__

int
main(void)
{
	FILE *unicode_txt;
	if ((unicode_txt = fopen("./unicode.txt", "r")) == NULL)
		err(1, "error: can't open ./unicode.txt");

	char *sqlerr = NULL;
	sqlite3 *db;
	if (sqlite3_open("./chars.db", &db))
		err(1, "error: can't open ./chars.db for writing.");

	/* we're assuming that the makefile has already purged the file */
	sqlite3_exec(db,
		SQL(
			CREATE TABLE map (id INTEGER PRIMARY KEY, description TEXT);
		),
	NULL, NULL, &sqlerr);
	if (sqlerr) errx(1, "sqlite3_exec1: %s\n", sqlerr);

	/* wrap inserts in transaction to improve insert speed */
	sqlite3_exec(db, SQL(PRAGMA synchronous = OFF), NULL, NULL, &sqlerr);
	if (sqlerr) errx(1, "sqlite3_exec2: %s\n", sqlerr);
	sqlite3_exec(db, SQL(PRAGMA journal_mode = MEMORY), NULL, NULL, &sqlerr);
	if (sqlerr) errx(1, "sqlite3_exec3: %s\n", sqlerr);
	sqlite3_exec(db, SQL(BEGIN TRANSACTION), NULL, NULL, &sqlerr);
	if (sqlerr) errx(1, "sqlite3_exec4: %s\n", sqlerr);

	ssize_t sqlerr2;

	char *insertq = SQL(
		INSERT INTO map VALUES (:c, :d);
	);

	sqlite3_stmt *insert_stmt;
	sqlerr2 = sqlite3_prepare_v2(db, insertq, strlen(insertq), &insert_stmt, NULL);
	if (sqlerr2) errx(1, "sqlite3_prepare_v2: %s\n", sqlite3_errmsg(db));

	ssize_t read = 0;
	size_t len = 0, linectr = 0;
	char *line = NULL;

	while ((read = getline(&line, (size_t*) &len, unicode_txt)) != -1) {
		char *tmp = NULL, *data[32];
		size_t i = 0;

		/* save pointer to line so that we can free it later */
		char *lineptr = line;
		while (++i, (tmp = strsep(&lineptr, ";"))) {
			data[i] = tmp;
		}

		char *desc = data[2];

		/*
		 * the "descriptions" for some characters aren't really
		 * descriptions, just '<control>'
		 * TODO: get proper descriptions for other control chars (like 0x81)
		 */
		if (linectr < 32) {
			desc = data[11];
		}

		/*
		 * descriptions in unicode.txt are uppercase
		 * and as we all know, uppercase is ugly
		 */
		size_t desclen = strlen(desc);
		for (i = 0; i < desclen; ++i)
			desc[i] = tolower(desc[i]);

		char id[32];
		sprintf((char*) &id, "%zu", linectr);

		sqlite3_bind_int(insert_stmt, 1, linectr);
		sqlite3_bind_text(insert_stmt, 2, desc, desclen, SQLITE_STATIC);
		sqlite3_step(insert_stmt);
		sqlite3_clear_bindings(insert_stmt);
		sqlite3_reset(insert_stmt);
		++linectr;
	}

	sqlite3_finalize(insert_stmt);
	sqlite3_exec(db, SQL(END TRANSACTION), NULL, NULL, &sqlerr);
	if (sqlerr) errx(1, "error: %s\n", sqlerr);
	sqlite3_exec(db, SQL(CREATE INDEX idx ON map (id);), NULL, NULL, &sqlerr);
	if (sqlerr) errx(1, "error: %s\n", sqlerr);

	sqlite3_close(db);
	free(line);
	fclose(unicode_txt);
}
