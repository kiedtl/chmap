CC      = cc
CFLAGS  = -O4 -s -I../sub/ccommon/
LDFLAGS = -lsqlite3
SRC     = upddb.c

all: unicode.txt clean chars.db

clean:
	rm -f upddb chars.db chars.db-journal

chars.db: upddb unicode.txt
	./upddb

upddb: $(SRC)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(SRC)

unicode.txt:
	@echo Oops, looks like unicode.txt is missing.
	@echo You\'ll have to retrieve it before you can
	@echo build lcharmap.
	@false

.PHONY: all clean
