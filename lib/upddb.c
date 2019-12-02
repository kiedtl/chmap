//
// upddb: update lcharmap character database (chars.db)
// (c) Kied Llaentenn and contributors
// see the LICENSE for more information.
//

// WARNING: very messy.
// I am in no wise an experienced C programmer
// and this code very likely contains serious bugs
// and/or memory leaks. Still, to the best of my 
// knowledge, this code is complete.

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Item
{
	int value;
	char *desc;
} Item;

int
main ( void )
{
	FILE *fp;
	char *line = NULL;
	size_t len = 0;
	size_t read;

	Item *items = (Item *) malloc(32842 * sizeof(Item)); // there are 32,842 items to process

	fp = fopen("./unicode.txt", "r");
	if (fp == NULL)
		exit(1);

	fprintf(stderr, "DEBUG: opened file; sizeof(items) = %i\n", sizeof(items));
	int ctr = 0;
	while ((read = getline(&line, &len, fp) != -1)) {
		fprintf(stderr, "DEBUG: read line %i of length %i\n", ctr, len);
		// parse into new Item
		// 255 (max desc size) + 6 (hex codepoint)
		char *token = (char *) malloc((255 + 6) * sizeof(char)); 
		int codepoint;
		char *desc = (char *) malloc(255 * sizeof(char));
		int tokctr = 0;
		while ((token = strsep(&line, ";"))) {
			if (tokctr == 0) {
				// convert hexadecimal codepoint to integer
				codepoint = (int)strtol(token, NULL, 16);
			}
			if (tokctr == 1) {
				strcpy(desc, token);
			}
			tokctr++;
		}
		
		// malloc for item.desc
		items[ctr].desc = (char *) malloc(sizeof(desc) * sizeof(char));
		strcpy(items[ctr].desc, "a");
		items[ctr].value = codepoint;
	
		// free stuff
		if (desc) free(desc);
		if (token) free(token);

		ctr++;
	}

	for (int i = 0; i < (sizeof(items)); i++) {
		printf("%i;%s", items[i].value, items[i].desc);
	}

	fclose(fp);
	if (line)
		free(line);

	free(items);
	exit(0);
}

char*
mystrcat ( char *dest, char *src )
{
	while (*dest)
		dest++;
	while ((*dest++ = *src++));
	return --dest;
}
