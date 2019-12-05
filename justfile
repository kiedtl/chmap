#
# lcharmap: a Linux port of the Windows `charmap` utility. 
# https://github.com/lptstr/lcharmap
#

# ----- VARIABLES -----

BUILDDIR  := "build"
PLATFORM  := `gcc -dumpmachine`

CARGOPTS := "build -j`nproc` --target-dir " + BUILDDIR
CARGOBIN := 'cargo'

CARGOPT_RELEASE := CARGOPTS + " --release --quiet"

# ----- RECIPES -----
all: debug

clean:
	rm -f "{{BUILDDIR}}/release/lcharmap"
	rm -f "{{BUILDDIR}}/debug/lcharmap"

dev-install:
	@echo "\tINSTALL\t\tbuild/debug/lcharmap"
	@install -m 755 "{{BUILDDIR}}/debug/lcharmap" "/usr/bin/lcharmap"

install: dbgen release
	@echo "\tINSTALL\t\tbuild/release/lcharmap"
	@install -m 755 "{{BUILDDIR}}/release/lcharmap" "/usr/bin/lcharmap"
	@install -m 644 "./lcharmap.1" "/usr/share/man/man1/lcharmap.1"

uninstall:
	rm -f "/usr/bin/lcharmap"

dbgen:
	@cd lib && just install

debug:
	@echo "\tRC\t\t{{CARGOPTS}}"
	@{{CARGOBIN}} {{CARGOPTS}}

release:
	@echo "\tRC\t\t{{CARGOPT_RELEASE}}"
	@{{CARGOBIN}} {{CARGOPT_RELEASE}}
