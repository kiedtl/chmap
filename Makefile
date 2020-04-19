#
# lcharmap: a Linux port of the Windows `charmap` utility. 
# https://github.com/lptstr/lcharmap
#
# (c) Kiëd Llaentenn and contributors
# See the LICENSE.md file for more information
#

DESTDIR =
PREFIX  = /usr/local

CMD     = @
VERSION = \"0.3.0\"

BIN     = lcharmap
SRC     = sub/argoat/src/argoat.c util.c dirs.c db.c terminfo.c $(BIN).c
OBJ     = $(SRC:.c=.o)
LIBUTF  = sub/libutf/lib/libutf.a
SQLITE  = sql/sqlite3.a

WARNING = -Wall -Wextra -pedantic -Wmissing-prototypes -Wold-style-definition \
	  -Wno-incompatible-pointer-types -Wno-unused-parameter \
	  -Wno-unused-value -Wno-trigraphs
INC     = -I. -Isub/ccommon/ -Isub/argoat/src/ -Isub/libutf/include/
DEF     = -DSQLITE_THREADSAFE=0 -DSQLITE_DEFAULT_MEMSTATUS=0

CC      = cc
LD      = gold
CFLAGS  = -std=c99 -DVERSION=$(VERSION) -D_DEFAULT_SOURCE $(WARNING) $(INC)
LDFLAGS = -lpthread -ldl -fuse-ld=$(LD)

all: debug

.c.o:
	@printf "    %-8s%s\n" "CC" $<
	$(CMD)$(CC) $(CFLAGS) $(CFLAGS_OPT) -c $< -o $(<:.c=.o)

debug: CFLAGS_OPT := -O0 -ggdb
debug: $(BIN)

release: CFLAGS_OPT  := -O3
release: LDFLAGS_OPT := -march=native -flto -s
release: $(BIN)

$(BIN): $(OBJ) $(LIBUTF) $(SQLITE)
	@printf "    %-8s%s\n" "CCLD" $@
	$(CMD)$(CC) -o $@ $^ $(CFLAGS) $(CFLAGS_OPT) $(LDFLAGS) $(LDFLAGS_OPT)

$(LIBUTF):
	$(CMD)cd sub/libutf && make

$(SQLITE): sql/sqlite3.c
	@printf "    %-8s%s\n" "CC" $^
	$(CMD)$(CC) -o $(@:.a=.o) -c $^ $(CFLAGS_OPT)
	@printf "    %-8s%s\n" "AR" $@
	$(CMD)$(AR) rvs $@ $(^:.c=.o)

lib/chars.db:
	@printf "    %-8s%s\n" "GEN" $@
	$(CMD)cd lib && make

$(BIN).1: $(BIN).scd
	@printf "    %-8s%s\n" "SCDOC" $@
	$(CMD)scdoc < $^ > $@

clean:
	$(CMD)rm -f $(BIN) $(OBJ)

install: $(BIN) $(BIN).1 lib/chars.db
	$(CMD)install -Dm755 $(BIN) $(DESTDIR)/$(PREFIX)/bin/$(BIN)
	$(CMD)install -Dm644 $(BIN).1 $(DESTDIR)/$(PREFIX)/share/man/man1/$(BIN).1
	$(CMD)install -Dm644 lib/chars.db $(HOME)/.local/share/$(BIN)/chars.db

uninstall:
	$(CMD)rm -f $(DESTDIR)/$(PREFIX)/bin/$(BIN)
	$(CMD)rm -f $(DESTDIR)/$(PREFIX)/share/man/man1/$(BIN).1
	$(CMD)rm -f $(HOME)/.local/share/$(BIN)/chars.db

.PHONY: all debug release clean install uninstall
