#ifndef TABLES_H
#define TABLES_H

#include "bool.h"
#include "types.h"
#include "vec.h"
#include "vecdef.h"


struct Table {
	u16 ttywidth;
	bool format_long;
	usize entry_len;
	vec_rune_t *entries;
	vec_str_t *descrips;
};

void table_show(struct Table *self);

#endif
