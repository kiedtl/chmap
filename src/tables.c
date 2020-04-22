#include <stdio.h>
#include <string.h>

#include "lcharmap.h"
#include "tables.h"
#include "utf.h"

static void table_print_line(struct Table *self);
static void table_print_header(struct Table *self);
static void table_print_entry(
	struct Table *self,
	Rune entry,
	char *description
);

void
table_print_line(struct Table *self)
{
	/* TODO: allow user to change line character
	 * via cmd options, just in case their terminal/font
	 * doesn't fully support UTF8 */

	/* don't forget the null terminator! */
	char line[((self->ttywidth) * sizeof("─")) + 1];
	strcpy(line, "─");
	for (usize i = 0; i < (usize) self->ttywidth - 1; ++i) {
	        strcat(line, "─");
	}
	line[self->ttywidth * sizeof("─")] = '\n';
	line[(self->ttywidth + 1) * sizeof("─")] = '\0';
	printf(line);
}

void
table_print_header(struct Table *self)
{
	table_print_line(self);
	printf("DEC\tHEX\tOCT\tHTML\tCHAR\tDESC\n");
	table_print_line(self);
}

void
table_print_entry(struct Table *self, Rune entry, char *description)
{
	char dec[snprintf(NULL, 0, "%d", entry)];
	sprintf((char*) &dec, "%d", entry);

	char hex[snprintf(NULL, 0, "0x%X", entry)];
	sprintf((char*) &hex, "0x%X", entry);

	char oct[snprintf(NULL, 0, "0b%o", entry)];
	sprintf((char*) &oct, "0b%o", entry);

	/*
	 * TODO: display "readable" html entities
	 * e.g. &amp; instead of &#38; for '&'
	 */
	char htm[snprintf(NULL, 0, "&#%d;", entry)];
	sprintf((char*) &htm, "&#%d;", entry);

	/* disable character field if entry is
	 * a control character */
	bool iscontrol = entry < 32 || (entry > 126 && entry < 160);

	usize sz;
	if (iscontrol) sz = sizeof("<control>");
	else sz = runelen(entry) + 1;
	char cha[sz];
	if (iscontrol) {
		strcpy((char*) &cha, "<control>");
	} else {
		runetochar((char*) &cha, &entry);
		cha[sz] = '\0';
	}

	if (self->format_long) {
		printf("%c[1m%20s  %c[m%s\n", 0x1B, "decimal", 0x1B,
			(char*) &dec);
		printf("%c[1m%20s  %c[m%s\n", 0x1B, "hexadecimal", 0x1B,
			(char*) &hex);
		printf("%c[1m%20s  %c[m%s\n", 0x1B, "octal", 0x1B,
			(char*) &oct);
		printf("%c[1m%20s  %c[m%s\n", 0x1B, "HTML entity", 0x1B,
			(char*) &htm);
		printf("%c[1m%20s  %c[m%s\n", 0x1B, "character", 0x1B,
			(char*) &cha);
		printf("%c[1m%20s  %c[m%s\n", 0x1B, "description", 0x1B,
			description);
		printf("\n");
	} else {
		printf("%s\t%s\t%s\t%s\t%s\t%s\n", &dec, &hex, &oct, &htm,
			&cha, description);
		table_print_line(self);
	}
}

void
table_show(struct Table *self)
{
	if (!self->format_long)
		table_print_header(self);

	for (usize i = 0; i < self->entry_len; ++i) {
		table_print_entry(
			self,
			self->entries->data[i],
			self->descrips->data[i]
		);
	}
}
