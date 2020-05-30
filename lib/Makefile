include ../config.mk

DEF     = -DSQLITE_THREADSAFE=0 -DSQLITE_DEFAULT_MEMSTATUS=0
INC = -I../sub/ccommon/include/ -I../sub/sql/
LDFLAGS = -lpthread -ldl -fuse-ld=$(LD)

all: unicode.txt clean chars.db

clean:
	rm -f upddb chars.db chars.db-journal

chars.db: upddb unicode.txt
	@printf "    %-8s%s\n" "GEN" $@
	$(CMD)./upddb

upddb: upddb.c ../sub/sql/sqlite3.a
	@printf "    %-8s%s\n" "CC" $@
	$(CMD)$(CC) -O3 -s $(LDFLAGS) $(DEF) $(INC) -o $@ $^

unicode.txt:
	@echo Oops, looks like unicode.txt is missing.
	@echo You\'ll have to retrieve it before you can
	@echo build lcharmap.
	@false

.PHONY: all clean
