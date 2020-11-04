#include "vecdef.h"
#include "vec.h"
#include "range.c"

#include <assert.h>
#include <stdio.h>

int
main(int argc, char **argv)
{
	vec_rune_t entries;
	vec_init(&entries);

	if (!expand_range(argv[1], &entries)) {
		fprintf(stderr, "invalid range\n");
		return 1;
	}

	printf("%d", entries.data[0]);
	for (size_t i = 1; i < entries.length; ++i) {
		printf(", %d", entries.data[i]);
	}

	printf("\n");
}
