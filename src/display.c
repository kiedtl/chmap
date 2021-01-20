#include <stdio.h>
#include <string.h>

#include "util.h"

#define CTRL "<control>"

static char *
fmt_bytes(char *bytes)
{
	static char buf[255];
	memset(buf, 0x0, sizeof(buf));

	for (size_t i = 0; bytes[i]; ++i)
		strcat(buf, format("%hhX ", bytes[i]));

	return (char *)&buf;
}

static void
fmt_entry(_Bool fancy, char *key, char *value)
{
	if (fancy)
		printf("\033[1m%-11s\033[m %s\n", key, value);
	else
		printf("%-11s %s\n", key, value);
}

static void
printentry(uint32_t entry, char *description, _Bool fancy)
{
	char charbuf[7];
	memset(charbuf, 0x0, sizeof(charbuf));

	utf8proc_encode_char(entry, (unsigned char *)charbuf);

	/* disable character field if entry is a control character */
	_Bool iscontrol = entry < 32 || (entry > 126 && entry < 160);

	fmt_entry(fancy, "codepoint",   format("%-5d 0x%-5X 0o%-5o", entry, entry, entry));
	fmt_entry(fancy, "UTF-8",       format("%s", fmt_bytes(charbuf)));
	fmt_entry(fancy, "glyph",       format("%s", iscontrol ? CTRL : charbuf));
	fmt_entry(fancy, "description", format("%s", description));
	printf("\n");
}
