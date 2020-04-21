/* TODO: fix nasty segfault here and use */
#include <ctype.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

#include "bool.h"
#include "range.h"
#include "types.h"
#include "util.h"

#define PARSE_INT(x, s, e) (x = strtol(s, e, 10), *e != s)

usize *accm;
usize idx;
static bool parse_range(char *s, char **e);

usize*
expand_range(char *s)
{
	/* save start point of input string
	 * so we can print it out in case of a
	 * syntax error */
	char *startptr = s;

	//accm = (usize*) ecalloc(255, sizeof(usize));
	idx = 0;

	usize x = 0;
	char **e = NULL;

	for (;;) {
		while (isspace(*s)) ++s;

		/*
		 * try to parse input as a range, and fall back
		 * to parsing input as a single integer if that
		 * failed.
		 * if both failed, it's probably a syntax error.
		 */
		if (!parse_range(s, e) && !PARSE_INT(x, s, e))
			break;
		s = *e;
		
		while (isspace(*s)) ++s;
		if ((*s) == '\0') return NULL;//return accm;

		/* check if there's something more to parse */
		if (*s == ',') {
			++s;
			continue;
		}

		break;
	}

	/* if we broke out of the main loop then a syntax
	 * error must have occurred */
	die("lcharmap: '%s': invalid range.", startptr);
	return NULL;
}

bool
parse_range(char *s, char **e)
{
	usize x = 0, y = 0;
	char *ee = NULL;

	/* try to parse left-hand side of range */
	if (!PARSE_INT(x, s, &ee))
		return FALSE;
	s = ee;
	while (isspace(*s)) ++s;

	/* check if this is really a range, or just
	 * a single integer */
	if (*s != '-') {
		*(char **) e = s;
		return FALSE;
	}
	
	++s;
	/* try to parse right-hand side of range */
	if (!PARSE_INT(y, s, e))
		return FALSE;

	/* check if left-hand size is greater than
	 * right-hand side of range */
	if (y <= x) return FALSE;

	/* copy onto accumulator */
	while (x <= y) printf("%d ", ++x);//accm[++idx] = ++x;
	return TRUE;
}
