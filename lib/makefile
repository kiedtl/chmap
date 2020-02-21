# TODO: add variables (CC, CFLAGS, etc)

all: clean chars.db

clean:
	rm -f upddb chars.db

upddb:
	gcc -O4 -s -std=c99 -Wall -pedantic upddb.c -o upddb

chars.db: upddb
	./upddb > chars.db
