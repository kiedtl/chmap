#ifndef UNICODE_H
#define UNICODE_H

#include <stdint.h>

#define UNICODE_MAX 0x10FFFF

enum {
	UC_Lu,
	UC_Ll,
	UC_Lt,
	UC_Mn,
	UC_Mc,
	UC_Me,
	UC_Nd,
	UC_Nl,
	UC_No,
	UC_Zs,
	UC_Zl,
	UC_Zp,
	UC_Cc,
	UC_Cf,
	UC_Cs,
	UC_Co,
	UC_Cn,

	UC_Lm,
	UC_Lo,
	UC_Pc,
	UC_Pd,
	UC_Ps,
	UC_Pe,
	UC_Pi,
	UC_Pf,
	UC_Po,
	UC_Sm,
	UC_Sc,
	UC_Sk,
	UC_So
};

enum {
	UBIDI_AL,
	UBIDI_AN,
	UBIDI_B,
	UBIDI_BN,
	UBIDI_CS,
	UBIDI_EN,
	UBIDI_ES,
	UBIDI_ET,
	UBIDI_FSI,
	UBIDI_L,
	UBIDI_LRE,
	UBIDI_LRI,
	UBIDI_LRO,
	UBIDI_NSM,
	UBIDI_ON,
	UBIDI_PDF,
	UBIDI_PDI,
	UBIDI_R,
	UBIDI_RLE,
	UBIDI_RLI,
	UBIDI_RLO,
	UBIDI_S,
	UBIDI_WS,
};

extern const char *category_strs[30];

struct CharInfo {
	uint8_t category;
	char bidirect;
	int32_t decimal;
	char *desc;
	int32_t upper;
	int32_t lower;
};

extern const struct CharInfo charinfos[UNICODE_MAX];
extern const uint8_t charwidths[UNICODE_MAX];

_Bool unicodeisupper(uint32_t c);
_Bool unicodeislower(uint32_t c);

#endif
