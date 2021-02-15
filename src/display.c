#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>

#include "util.h"
#include "unicode.h"
#include "utf8.h"

static char *
fmt_bytes(char *bytes)
{
	static char buf[255];
	memset(buf, 0x0, sizeof(buf));

	size_t i = 0;
	for (; bytes[i]; ++i)
		strcat(buf, format("%hhX ", bytes[i]));

	buf[(i * 3) - 1] = '\0';
	return (char *)&buf;
}

static void
printentry_short(uint32_t entry, char *description)
{
	char charbuf[7];
	bzero(charbuf, sizeof(charbuf));

	utf8_unicode_to_char(charbuf, entry);

	size_t category = charinfos[entry].category;
	_Bool iscontrol = category == UC_Cc;

	printf("%9d  %s\t  %-11s  %s  %s\n",
		entry, iscontrol ? "" : charbuf, fmt_bytes(charbuf),
		utf8isupper(entry) ? "upper" : (utf8islower(entry) ? "lower" : "other"),
		description ? description : "-");
}

static void
fmt_entry(_Bool fancy, char *key, char *value)
{
	if (fancy)
		printf("\033[1m%-12s\033[m %s\n", key, value);
	else
		printf("%-12s %s\n", key, value);
}

static void
printentry_long(uint32_t entry, char *description, _Bool fancy)
{
	char charbuf[7], charbuf2[7];
	bzero(charbuf, sizeof(charbuf));
	bzero(charbuf2, sizeof(charbuf2));

	utf8_unicode_to_char(charbuf, entry);

	size_t colwidth = charwidths[entry];
	struct CharInfo ci = charinfos[entry];
	_Bool iscontrol = ci.category == UC_Cc;

	fmt_entry(fancy, "codepoint",   format("%-5d 0x%-5X 0o%-5o", entry, entry, entry));
	fmt_entry(fancy, "encoding",    format("UTF8(%s)", fmt_bytes(charbuf)));
	fmt_entry(fancy, "glyph",       format("%s (%zd %s)", iscontrol ? "<control>" : charbuf,
				colwidth, colwidth == 1 ? "column" : "columns"));
	fmt_entry(fancy, "description", description ? description : "(none)");

	if (utf8isupper(entry)) {
		int32_t lower = charinfos[entry].lower;
		assert(lower != -1);
		utf8_unicode_to_char(charbuf, lower);
		fmt_entry(fancy, "case",
			format("uppercase, lowercase: 0x%X %s", lower, charbuf2));
	} else if (utf8islower(entry)) {
		int32_t upper = charinfos[entry].upper;
		assert(upper != -1);
		utf8_unicode_to_char(charbuf, upper);
		fmt_entry(fancy, "case",
			format("lowercase, uppercase: 0x%X %s", upper, charbuf2));
	} else {
		fmt_entry(fancy, "case", "other");
	}

	fmt_entry(fancy, "category", (char *)category_strs[ci.category]);

	printf("\n");
}

static void
printheader(_Bool flong)
{
	if (flong)
		return;

	printf("codepoint  glyph  encoded      case   description\n");
}

static void
printentry(uint32_t entry, char *description, _Bool fancy, _Bool flong)
{
	if (!flong)
		printentry_short(entry, description);
	else
		printentry_long(entry, description, fancy);
}
