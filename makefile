#
# lcharmap: a Linux port of the Windows `charmap` utility. 
# https://github.com/lptstr/lcharmap
#

CARGOPTS = build -j`nproc`
CARGOBIN = cargo


all: debug

clean:
	rm -f "target/release/lcharmap"
	rm -f "target/debug/lcharmap"

dev-install: target/debug/lcharmap
	install -m 755 target/debug/lcharmap /usr/bin/lcharmap

install: target/release/lcharmap lcharmap.1 lib/chars.db
	install -Dm755 "target/release/lcharmap" "/usr/bin/lcharmap"
	install -Dm644 "./lcharmap.1" "/usr/share/man/man1/lcharmap.1"
	install -Dm644 "lib/chars.db" "$(HOME)/.local/share/lcharmap/chars.db"

uninstall:
	rm -f /usr/bin/lcharmap
	rm -f /usr/share/man/man1/lcharmap.1
	rm -f $(HOME)/.local/share/lcharmap/chars.db


chars.db:
	@cd lib && make

target/debug/lcharmap:
	cargo $(CARGOPTS)

target/release/lcharmap:
	cargo $(CARGOPTS) --release

lcharmap.1: lcharmap.sc
	scdoc < lcharmap.sc > lcharmap.1

.PHONY: all clean dev-install install uninstall
