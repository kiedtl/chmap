#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

#include "bool.h"
#include "range.h"
#include "types.h"
#include "util.h"
#include "vec.h"
#include "vecdef.h"

static bool parse_range(char *s, char **e, vec_rune_t *entries);
static bool parse_int(int x, char *s, char **e, bool add, vec_rune_t *entries);

bool
expand_range(char *s, vec_rune_t *entries)
{
	/* save start point of input string
	 * so we can print it out in case of a
	 * syntax error */
	char *startptr = s;

	int x;
	char **e;

	for (;;) {
		/*
		 * try to parse input as a range, and fall back
		 * to parsing input as a single integer if that
		 * failed.
		 * if both failed, it's probably a syntax error.
		 */
		if (!parse_range(s, e, entries)
				&& !parse_int(x, s, e, TRUE, entries))
			break;
		s = *e;
		
		if ((*s) == '\0') return TRUE;

		/* check if there's something more to parse */
		if ((*s) == ',') {
			++s;
			continue;
		}

		break;
	}

	/* if we broke out of the main loop then a syntax
	 * error must have occurred */
	return FALSE;
}

bool
parse_range(char *s, char **e, vec_rune_t *entries)
{
	int x, y;
	char *ee;

	/* try to parse left-hand side of range */
	if (!parse_int(x, s, &ee, FALSE, entries))
		return FALSE;
	s = ee;

	/* check if this is really a range, or just
	 * a single integer */
	if (*s != '-') {
		*(char **) e = s;
		return FALSE;
	}
	
	++s;
	/* try to parse right-hand side of range */
	if (!parse_int(y, s, e, FALSE, entries))
		return FALSE;

	/* check if left-hand size is greater than
	 * right-hand side of range */
	if (y <= x) return FALSE;

	/* copy onto accumulator */
	for (usize i = x; i <= y; ++i)
		vec_push(entries, (Rune) i);
	return TRUE;
}

bool
parse_int(int x, char *s, char **e, bool add, vec_rune_t *entries)
{
	/* TODO: bases */
	x = strtol(s, e, 10);
	bool ok = *e != s;

	/* HACK: the add parameter controls whether
	 * a parsed integer is added to the entries
	 * if it succeeds in parsing it.
	 *
	 * The reason it is needed is because parsed_int
	 * is used in two places: 1) in expand_range (where
	 * we *want* successfully parse integers to be
	 * added to the entries) and 2) in parse_range
	 * (where we *don't want* successfully parsed integers
	 * to be added to the entries.
	 */

	if (ok && add) vec_push(entries, (Rune) x);
	return ok;
}
