#include <stdio.h>
#include <string.h>

#include "lcharmap.h"
#include "tables.h"
#include "utf.h"

void
r_table_print_entry(Rune entry, char *description)
{
	char dec[snprintf(NULL, 0, "%d", entry)];
	sprintf((char*) &dec, "%d", entry);

	char hex[snprintf(NULL, 0, "0x%X", entry)];
	sprintf((char*) &hex, "0x%X", entry);

	char oct[snprintf(NULL, 0, "0o%o", entry)];
	sprintf((char*) &oct, "0o%o", entry);

	/* disable character field if entry is a control character */
	usize sz;
	bool iscontrol = entry < 32 || (entry > 126 && entry < 160);
	if (iscontrol) {
		sz = sizeof("<ctrl>");
	} else {
		sz = runelen(entry) + 1;
	}

	char cha[sz];
	if (iscontrol) {
		strcpy((char*) &cha, "<ctrl>");
	} else {
		runetochar((char*) &cha, &entry);
		cha[sz-1] = '\0';
	}

	printf("%c[1m%11s  %c[m%s\n", 0x1B, "decimal", 0x1B,
		(char*) &dec);
	printf("%c[1m%11s  %c[m%s\n", 0x1B, "hex", 0x1B,
		(char*) &hex);
	printf("%c[1m%11s  %c[m%s\n", 0x1B, "oct", 0x1B,
		(char*) &oct);
	printf("%c[1m%11s  %c[m%s\n", 0x1B, "glyph", 0x1B,
		(char*) &cha);
	printf("%c[1m%11s  %c[m%s\n", 0x1B, "description", 0x1B,
		description);
	printf("\n");
}
