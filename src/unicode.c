#include "unicode.h"

const char *category_strs[30] = {
	[UC_Cn] = "Other (not assigned)",
	[UC_Lu] = "Letter (uppercase)",
	[UC_Ll] = "Letter (lowercase)",
	[UC_Lt] = "Letter (titlecase)",
	[UC_Lm] = "Letter (modifier)",
	[UC_Lo] = "Letter (other)",
	[UC_Mn] = "Mark (nonspacing)",
	[UC_Mc] = "Mark (space combining)",
	[UC_Me] = "Mark (enclosing)",
	[UC_Nd] = "Number (decimal digit)",
	[UC_Nl] = "Number (letter)",
	[UC_No] = "Number (other)",
	[UC_Pc] = "Punctuation (connector)",
	[UC_Pd] = "Punctuation (dash)",
	[UC_Ps] = "Punctuation (open)",
	[UC_Pe] = "Punctuation (close)",
	[UC_Pi] = "Punctuation (initial quote)",
	[UC_Pf] = "Punctuation (final quote)",
	[UC_Po] = "Punctuation (other)",
	[UC_Sm] = "Symbol (math)",
	[UC_Sc] = "Symbol (currency)",
	[UC_Sk] = "Symbol (modifier)",
	[UC_So] = "Symbol (other)",
	[UC_Zs] = "Separator (space)",
	[UC_Zl] = "Separator (line)",
	[UC_Zp] = "Separator (paragraph)",
	[UC_Cc] = "Other (control)",
	[UC_Cf] = "Other (format)",
	[UC_Cs] = "Other (surrogate)",
	[UC_Co] = "Other (private use)",
};

#include "dat/charinfo.c"
#include "dat/charwidths.c"

_Bool
unicodeisupper(uint32_t c)
{
	struct CharInfo ci = charinfos[c];
	return ci.lower != ci.upper && ci.upper == -1 && ci.category != UC_Lt;
}

_Bool
unicodeislower(uint32_t c)
{
	struct CharInfo ci = charinfos[c];
	return ci.lower != ci.upper && ci.lower == -1;
}
