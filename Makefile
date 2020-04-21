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
SRC     = sub/arg/argoat.c src/util.c src/dirs.c \
	  src/db.c src/terminfo.c src/$(BIN).c
OBJ     = $(subst src/,obj/,$(SRC:.c=.o))

LIBUTF  = sub/libutf/lib/libutf.a
SQLITE  = sub/sql/sqlite3.a

WARNING = -Wall -Wextra -pedantic -Wmissing-prototypes -Wold-style-definition \
	  -Wno-incompatible-pointer-types -Wno-unused-parameter \
	  -Wno-unused-value -Wno-trigraphs
INC     = -I. -Isub/ccommon/ -Isub/arg/ -Isub/libutf/include/ -Isub/sql/
DEF     = -DSQLITE_THREADSAFE=0 -DSQLITE_DEFAULT_MEMSTATUS=0

AR      = ar
CC      = cc
LD      = gold
CFLAGS  = -std=c99 -DVERSION=$(VERSION) -D_DEFAULT_SOURCE $(WARNING) $(INC)
LDFLAGS = -lpthread -ldl -fuse-ld=$(LD)

all: man/$(BIN).1 debug

obj/:
	$(CMD)mkdir -p obj

.o.a:
	@printf "    %-8s%s\n" "AR" $<
	$(CMD)$(AR) rvs $@ $(^:.c=.o) >/dev/null 2>&1

obj/%.o: src/%.c obj/
	@printf "    %-8s%s\n" "CC" $(subst src/,obj/,$(<:.c=.o))
	$(CMD)$(CC) $(CFLAGS) $(CFLAGS_OPT) -c $< \
		-o $(subst src/,obj/,$(<:.c=.o))

debug: CFLAGS_OPT := -O0 -ggdb
debug: obj/$(BIN)

release: CFLAGS_OPT  := -O3
release: LDFLAGS_OPT := -march=native -flto -s
release: obj/$(BIN)

obj/$(BIN): $(OBJ) $(LIBUTF) $(SQLITE)
	@printf "    %-8s%s\n" "CCLD" $@
	$(CMD)$(CC) -o $@ $^ $(CFLAGS) $(CFLAGS_OPT) $(LDFLAGS) $(LDFLAGS_OPT)

$(LIBUTF):
	$(CMD)cd sub/libutf && make

lib/chars.db:
	@printf "    %-8s%s\n" "GEN" $@
	$(CMD)cd lib && make

man/$(BIN).1: man/$(BIN).scd
	@printf "    %-8s%s\n" "SCDOC" $@
	$(CMD)scdoc < $^ > $@

clean:
	$(CMD)rm -f obj/$(BIN) $(OBJ) man/$(BIN).1

install: $(BIN) $(BIN).1 lib/chars.db
	$(CMD)install -Dm755 obj/$(BIN) $(DESTDIR)/$(PREFIX)/bin/$(BIN)
	$(CMD)install -Dm644 man/$(BIN).1 $(DESTDIR)/$(PREFIX)/share/man/man1/$(BIN).1
	$(CMD)install -Dm644 lib/chars.db $(HOME)/.local/share/$(BIN)/chars.db

uninstall:
	$(CMD)rm -f $(DESTDIR)/$(PREFIX)/bin/$(BIN)
	$(CMD)rm -f $(DESTDIR)/$(PREFIX)/share/man/man1/$(BIN).1
	$(CMD)rm -f $(HOME)/.local/share/$(BIN)/chars.db

.PHONY: all debug release clean install uninstall