#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <utf8proc.h>

#include "util.h"

static const char *CATEGORY_STRS[] = {
	[UTF8PROC_CATEGORY_CN] = "Other (not assigned)",
	[UTF8PROC_CATEGORY_LU] = "Letter (uppercase)",
	[UTF8PROC_CATEGORY_LL] = "Letter (lowercase)",
	[UTF8PROC_CATEGORY_LT] = "Letter (titlecase)",
	[UTF8PROC_CATEGORY_LM] = "Letter (modifier)",
	[UTF8PROC_CATEGORY_LO] = "Letter (other)",
	[UTF8PROC_CATEGORY_MN] = "Mark (nonspacing)",
	[UTF8PROC_CATEGORY_MC] = "Mark (space combining)",
	[UTF8PROC_CATEGORY_ME] = "Mark (enclosing)",
	[UTF8PROC_CATEGORY_ND] = "Number (decimal digit)",
	[UTF8PROC_CATEGORY_NL] = "Number (letter)",
	[UTF8PROC_CATEGORY_NO] = "Number (other)",
	[UTF8PROC_CATEGORY_PC] = "Punctuation (connector)",
	[UTF8PROC_CATEGORY_PD] = "Punctuation (dash)",
	[UTF8PROC_CATEGORY_PS] = "Punctuation (open)",
	[UTF8PROC_CATEGORY_PE] = "Punctuation (close)",
	[UTF8PROC_CATEGORY_PI] = "Punctuation (initial quote)",
	[UTF8PROC_CATEGORY_PF] = "Punctuation (final quote)",
	[UTF8PROC_CATEGORY_PO] = "Punctuation (other)",
	[UTF8PROC_CATEGORY_SM] = "Symbol (math)",
	[UTF8PROC_CATEGORY_SC] = "Symbol (currency)",
	[UTF8PROC_CATEGORY_SK] = "Symbol (modifier)",
	[UTF8PROC_CATEGORY_SO] = "Symbol (other)",
	[UTF8PROC_CATEGORY_ZS] = "Separator (space)",
	[UTF8PROC_CATEGORY_ZL] = "Separator (line)",
	[UTF8PROC_CATEGORY_ZP] = "Separator (paragraph)",
	[UTF8PROC_CATEGORY_CC] = "Other (control)",
	[UTF8PROC_CATEGORY_CF] = "Other (format)",
	[UTF8PROC_CATEGORY_CS] = "Other (surrogate)",
	[UTF8PROC_CATEGORY_CO] = "Other (private use)",
};

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
	char charbuf[7], charbuf2[7];
	bzero(charbuf, sizeof(charbuf));
	bzero(charbuf2, sizeof(charbuf2));

	utf8proc_encode_char(entry, (unsigned char *)charbuf);

	const utf8proc_property_t *prop = utf8proc_get_property((int32_t)entry);
	_Bool iscontrol = prop->category == UTF8PROC_CATEGORY_CC;

	printf("\t0x%X\t%d  %s\t%s\t%s\t%s     %s\n",
		entry, entry, iscontrol ? "<ctrl>" : charbuf, fmt_bytes(charbuf),
		utf8proc_isupper(entry) ? "upper" : (utf8proc_islower(entry) ? "lower" : "other"),
		utf8proc_category_string(prop->category), description);
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
printentry_long(uint32_t entry, char *description, _Bool fancy)
{
	char charbuf[7], charbuf2[7];
	bzero(charbuf, sizeof(charbuf));
	bzero(charbuf2, sizeof(charbuf2));

	utf8proc_encode_char(entry, (unsigned char *)charbuf);

	ssize_t colwidth = utf8proc_charwidth((int32_t)entry);
	const char *colwidthstr = colwidth == 1 ? "column" : "columns";
	const utf8proc_property_t *prop = utf8proc_get_property((int32_t)entry);
	_Bool iscontrol = prop->category == UTF8PROC_CATEGORY_CC;

	fmt_entry(fancy, "codepoint",   format("%-5d 0x%-5X 0o%-5o", entry, entry, entry));
	fmt_entry(fancy, "encoding",    format("UTF8(%s)", fmt_bytes(charbuf)));
	fmt_entry(fancy, "glyph",       format("%s (%zd %s)", iscontrol ? "<control>" : charbuf,
				colwidth, colwidthstr));
	fmt_entry(fancy, "description", format("%s", description));

	if (utf8proc_isupper((int32_t)entry)) {
		utf8proc_int32_t lower = utf8proc_tolower((int32_t)entry);
		utf8proc_encode_char(lower, (unsigned char *)charbuf2);
		fmt_entry(fancy, "case",
			format("uppercase, lowercase: 0x%X %s", lower, charbuf2));
	} else if (utf8proc_islower((int32_t)entry)) {
		utf8proc_int32_t upper = utf8proc_toupper((int32_t)entry);
		utf8proc_encode_char(upper, (unsigned char *)charbuf2);
		fmt_entry(fancy, "case",
			format("lowercase, uppercase: 0x%X %s", upper, charbuf2));
	} else {
		fmt_entry(fancy, "case", "other");
	}

	fmt_entry(fancy, "category",    (char *)CATEGORY_STRS[prop->category]);

	printf("\n");
}

static void
printentry(uint32_t entry, char *description, _Bool fancy, _Bool flong)
{
	if (!flong)
		printentry_short(entry, description);
	else
		printentry_long(entry, description, fancy);
}
