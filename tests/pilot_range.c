#include <stdio.h>
#include "range.c"

int
main(int argc, char **argv)
{
	uint32_t entries[4096] = {0};
	ssize_t  entries_len = 0;
	if ((entries_len = expand_range(argv[1], entries)) < 0)
		return 1;

	printf("%d", entries[0]);
	for (size_t i = 1; i < entries_len; ++i) {
		printf(" %d", entries[i]);
	}

	printf("\n");
}
