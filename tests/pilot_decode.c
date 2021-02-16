#include <stdio.h>
#include <string.h>
#include "utf8.c"

int
main(int argc, char **argv)
{
	uint32_t charbuf = 0;
	ssize_t runelen  = 0;

	while (*argv[1]) {
		charbuf = 0;
		if ((runelen = utf8_decode(&charbuf, argv[1], strlen(argv[1]))) < 0)
			return 1;
		printf("U+%04X\n", charbuf);
		argv[1] += runelen;
	}
}
