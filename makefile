#
# lcharmap: a Linux port of the Windows `charmap` utility. 
# https://github.com/lptstr/lcharmap
#

DESTDIR =
PREFIX  = /usr/local

VERSION = \"0.3.0\"

BIN     = lcharmap
SRC     = sub/argoat/src/argoat.c util.c dirs.c db.c terminfo.c $(BIN).c
OBJ     = $(SRC:.c=.o)

WARNING = -Wall -Wextra -pedantic -Wmissing-prototypes -Wno-unused-parameter \
	  -Wold-style-definition -Wno-incompatible-pointer-types
INC     = -I. -Isub/ccommon/ -Isub/argoat/src/

CC      = cc
LD      = gold
CFLAGS  = -std=c99 -DVERSION=$(VERSION) -D_DEFAULT_SOURCE $(WARNING) $(INC)
LDFLAGS = -lsqlite3 -fuse-ld=$(LD)

all: debug

.c.o:
	$(CC) $(CFLAGS) $(CFLAGS_OPT) -c $< -o $(<:.c=.o)

debug: CFLAGS_OPT := -O0 -ggdb
debug: $(BIN)

release: CFLAGS_OPT := -O4 -s
release: $(BIN)

$(BIN): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(CFLAGS_OPT) $(LDFLAGS)

lib/chars.db:
	@cd lib && make

$(BIN).1: $(BIN).scd
	scdoc < $^ > $@

clean:
	rm -f $(BIN) $(OBJ)


install: $(BIN) $(BIN).1 lib/chars.db
	install -Dm755 $(BIN) $(DESTDIR)/$(PREFIX)/bin/$(BIN)
	install -Dm644 $(BIN).1 $(DESTDIR)/$(PREFIX)/share/man/man1/$(BIN).1
	install -Dm644 lib/chars.db $(HOME)/.local/share/$(BIN)/chars.db

uninstall:
	rm -f $(DESTDIR)/$(PREFIX)/bin/$(BIN)
	rm -f $(DESTDIR)/$(PREFIX)/share/man/man1/$(BIN).1
	rm -f $(HOME)/.local/share/$(BIN)/chars.db

.PHONY: all debug release clean install uninstall
