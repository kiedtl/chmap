#ifndef TABLES_H
#define TABLES_H

#include "types.h"
#include "utf.h"

void table_print_line(u16 termwidth);
void table_print_header(void);
void table_print_entry(Rune entry, char *description);

#endif
