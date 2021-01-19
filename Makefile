#
# lcharmap: retreive information about Unicode characters
# https://github.com/lptstr/lcharmap
#
# (c) Kiëd Llaentenn and contributors
# See the LICENSE.md file for more information
#

include config.mk

VERSION = 1.0.0

BIN     = lcharmap
SRC     = sub/arg/argoat.c src/util.c src/dirs.c src/db.c src/$(BIN).c
OBJ     = $(SRC:.c=.o)

ARGOAT  = sub/arg/argoat.a
LIBUTF  = sub/libutf/lib/libutf.a
SQLITE  = sub/sql/sqlite3.a

WARNING = -Wall -Wpedantic -Wextra -Wold-style-definition \
	  -Wmissing-prototypes -Winit-self -Wfloat-equal -Wstrict-prototypes \
	  -Wredundant-decls -Wendif-labels -Wstrict-aliasing=2 -Woverflow \
	  -Wformat=2 -Wmissing-include-dirs
INC     = -I. -Isub/arg/ -Isub/libutf/include/ -Isub/sql/ -Isub/vec/src
DEF     = -DSQLITE_THREADSAFE=0 -DSQLITE_DEFAULT_MEMSTATUS=0 \
	  -D_DEFAULT_SOURCE -D_XOPEN_SOURCE=500 -D_POSIX_C_SOURCE=200809L
CFLAGS  = -std=c99 -DVERSION=\"$(VERSION)\" $(WARNING) $(INC)
LDFLAGS = -lpthread -ldl -fuse-ld=$(LD)

all: man/$(BIN).1 debug

.c.o:
	@printf "    %-8s%s\n" "CC" $@
	$(CMD)$(CC) $(CFLAGS) $(CFLAGS_OPT) -c $< -o $(<:.c=.o)

debug: CFLAGS_OPT  := $(DEBUG_CFLAGS)
debug: LDFLAGS_OPT := $(DEBUG_LDFLAGS)
debug: $(BIN)

release: CFLAGS_OPT  := $(RELEASE_CFLAGS)
release: LDFLAGS_OPT := $(RELEASE_LDFLAGS)
release: $(BIN)

$(BIN): $(OBJ) $(LIBUTF) $(SQLITE)
	@printf "    %-8s%s\n" "CCLD" $@
	$(CMD)$(CC) -o $@ $^ $(CFLAGS) $(CFLAGS_OPT) $(LDFLAGS) $(LDFLAGS_OPT)

$(ARGOAT):
	@printf "    %-8s%s\n" "MAKE" $@
	$(CMD)cd sub/arg && make

$(LIBUTF):
	@printf "    %-8s%s\n" "MAKE" $@
	$(CMD)cd sub/libutf && make
$(SQLITE):
	@printf "    %-8s%s\n" "MAKE" $@
	$(CMD)cd sub/sql && make

lib/chars.db:
	@printf "    %-8s%s\n" "GEN" $@
	$(CMD)cd sub/lib && make

man/$(BIN).1: man/$(BIN).scd
	@printf "    %-8s%s\n" "SCDOC" $@
	$(CMD)scdoc < $^ > $@

clean:
	$(CMD)rm -f $(BIN) $(OBJ) man/$(BIN).1
	$(CMD)make -C sub/arg clean
	$(CMD)make -C sub/libutf clean
	$(CMD)make -C sub/sql clean
	$(CMD)rm -rf dist/ *.xz

dist: clean
	$(CMD)mkdir -p dist
	$(CMD)cp -r config.mk Makefile *.md lib man src sub \
		dist
	$(CMD)tar -cvf - dist | xz -qcT 0 > lcharmap-v$(VERSION).tar.xz
	$(CMD)rm -rf dist

install: $(BIN) man/$(BIN).1 lib/chars.db
	$(CMD)install -Dm755 $(BIN) $(DESTDIR)/$(PREFIX)/bin/$(BIN)
	$(CMD)install -Dm644 man/$(BIN).1 $(DESTDIR)/$(PREFIX)/share/man/man1/$(BIN).1
	$(CMD)install -Dm644 lib/chars.db $(HOME)/.local/share/$(BIN)/chars.db

uninstall:
	$(CMD)rm -f $(DESTDIR)/$(PREFIX)/bin/$(BIN)
	$(CMD)rm -f $(DESTDIR)/$(PREFIX)/share/man/man1/$(BIN).1
	$(CMD)rm -f $(HOME)/.local/share/$(BIN)/chars.db

.PHONY: all debug release clean dist install uninstall
