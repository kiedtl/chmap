#
# chmap: retreive information about Unicode characters
# https://github.com/lptstr/chmap
#
# (c) Kiëd Llaentenn and contributors
# See the LICENSE.md file for more information
#

include config.mk

BIN     = chmap
VERSION = 1.1.0
PKGNAME = $(BIN)-$(shell uname -s)-$(shell uname -m)-$(VERSION)

SRC     = src/util.c src/dirs.c src/db.c src/main.c
OBJ     = $(SRC:.c=.o)

WARNING = -Wall -Wpedantic -Wextra -Wold-style-definition -Wformat=2 \
	  -Wmissing-prototypes -Winit-self -Wfloat-equal -Wstrict-prototypes \
	  -Wredundant-decls -Wendif-labels -Wstrict-aliasing=2 -Woverflow \
	  -Werror=missing-include-dirs -Werror=implicit-function-declaration \
	  -Werror=return-type

INC     = -I. -I ~/local/include/
DEF     = -DSQLITE_THREADSAFE=0 -DSQLITE_DEFAULT_MEMSTATUS=0 \
	  -D_DEFAULT_SOURCE -D_XOPEN_SOURCE=500 -D_POSIX_C_SOURCE=200809L \
	  -DVERSION=\"$(VERSION)\"

CFLAGS  = -std=c99 $(WARNING) $(INC) $(DEF)
LDFLAGS = -L ~/local/lib -lpthread -ldl -lsqlite3 -lutf8proc -fuse-ld=$(LD)

all: man/$(BIN).1 debug

.c.o:
	@printf "    %-8s%s\n" "CC" $@
	$(CMD)$(CC) $(CFLAGS) $(CFLAGS_OPT) -c $< -o $(<:.c=.o)

debug: CFLAGS_OPT  := $(DEBUG_CFLAGS)
debug: LDFLAGS_OPT := $(DEBUG_LDFLAGS)
debug: $(BIN)

release: CFLAGS_OPT  := $(RELEASE_CFLAGS)
release: LDFLAGS_OPT := $(RELEASE_LDFLAGS)
release: $(BIN) lib/chars.db man/$(BIN).1

src/main.o: src/range.c src/display.c

$(BIN): $(OBJ) $(LIBUTF)
	@printf "    %-8s%s\n" "CCLD" $@
	$(CMD)$(CC) -o $@ $^ $(CFLAGS) $(CFLAGS_OPT) $(LDFLAGS) $(LDFLAGS_OPT)

lib/chars.db:
	@printf "    %-8s%s\n" "MAKE" $@
	$(CMD)cd lib && make

man/$(BIN).1: man/$(BIN).scd
	@printf "    %-8s%s\n" "SCDOC" $@
	$(CMD)scdoc < $^ > $@

clean:
	rm -f $(BIN) $(OBJ) man/$(BIN).1
	rm -rf *.xz $(PKGNAME)*
	cd lib && make clean

dist: release man/$(NAME).1
	$(CMD)mkdir $(PKGNAME)
	$(CMD)cp man/$(NAME)   $(PKGNAME)
	$(CMD)cp man/$(NAME).1 $(PKGNAME)
	$(CMD)tar -cf - $(PKGNAME) | xz -qcT0 > $(PKGNAME).tar.xz
	$(CMD)rm -rf $(PKGNAME)


install: $(BIN) man/$(BIN).1 lib/chars.db
	install -Dm755 $(BIN) $(DESTDIR)/$(PREFIX)/bin/$(BIN)
	install -Dm644 man/$(BIN).1 $(DESTDIR)/$(PREFIX)/share/man/man1/$(BIN).1
	install -Dm644 lib/chars.db $(HOME)/.local/share/$(BIN)/chars.db

uninstall:
	rm -f $(DESTDIR)/$(PREFIX)/bin/$(BIN)
	rm -f $(DESTDIR)/$(PREFIX)/share/man/man1/$(BIN).1
	rm -f $(HOME)/.local/share/$(BIN)/chars.db

.PHONY: all debug release clean dist install uninstall
