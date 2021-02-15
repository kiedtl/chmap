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

SRC     = src/util.c src/main.c
OBJ     = $(SRC:.c=.o)

WARNING = -Wall -Wpedantic -Wextra -Wold-style-definition -Wformat=2 \
	  -Wmissing-prototypes -Winit-self -Wfloat-equal -Wstrict-prototypes \
	  -Wredundant-decls -Wendif-labels -Wstrict-aliasing=2 -Woverflow \
	  -Werror=implicit-function-declaration -Werror=return-type

INC     = -I ~/local/include/ -Itool/
DEF     = -D_DEFAULT_SOURCE -D_XOPEN_SOURCE=500 -D_POSIX_C_SOURCE=200809L \
	  -DVERSION=\"$(VERSION)\"

CFLAGS  = -std=c99 $(WARNING) $(INC) $(DEF)
LDFLAGS = -fuse-ld=$(LD) -L ~/local/lib -lutf8proc

all: man/$(BIN).1 debug

.c.o:
	@printf "    %-8s%s\n" "CC" $@
	$(CMD)$(CC) $(CFLAGS) $(CFLAGS_OPT) -c $< -o $(<:.c=.o)

debug: CFLAGS_OPT  := $(DEBUG_CFLAGS)
debug: LDFLAGS_OPT := $(DEBUG_LDFLAGS)
debug: $(BIN)

release: CFLAGS_OPT  := $(RELEASE_CFLAGS)
release: LDFLAGS_OPT := $(RELEASE_LDFLAGS)
release: $(BIN) man/$(BIN).1

src/main.o: tool/chars.h src/range.c src/display.c

$(BIN): tool/chars.h $(OBJ)
	@printf "    %-8s%s\n" "CCLD" $@
	$(CMD)$(CC) -o $@ $(OBJ) $(CFLAGS) $(CFLAGS_OPT) $(LDFLAGS) $(LDFLAGS_OPT)

tool/chars.h: tool/gencharsh.lua dat/UnicodeData.txt
	@printf "    %-8s%s\n" "GEN" $@
	$(CMD)tool/gencharsh.lua < dat/UnicodeData.txt > $@

man/$(BIN).1: man/$(BIN).scd
	@printf "    %-8s%s\n" "SCDOC" $@
	$(CMD)scdoc < $^ > $@

clean:
	rm -f $(BIN) $(OBJ) man/$(BIN).1
	rm -rf *.xz $(PKGNAME)*
	rm tool/chars.h

dist: release man/$(BIN).1
	$(CMD)mkdir $(PKGNAME)
	$(CMD)cp $(BIN)          $(PKGNAME)
	$(CMD)cp man/$(BIN).1    $(PKGNAME)
	$(CMD)cp tool/install.sh $(PKGNAME)
	$(CMD)tar -cf - $(PKGNAME) | xz -qcT0 > $(PKGNAME).tar.xz
	$(CMD)rm -rf $(PKGNAME)


install: $(BIN) man/$(BIN).1
	install -Dm755 $(BIN) $(DESTDIR)/$(PREFIX)/bin/$(BIN)
	install -Dm644 man/$(BIN).1 $(DESTDIR)/$(PREFIX)/share/man/man1/$(BIN).1

uninstall:
	rm -f $(DESTDIR)/$(PREFIX)/bin/$(BIN)
	rm -f $(DESTDIR)/$(PREFIX)/share/man/man1/$(BIN).1

.PHONY: all debug release clean dist install uninstall
