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

PREFIX := '/usr'
DESTDIR := '/bin'

# ----- RECIPES -----
all: debug

clean:
	rm -f "{{BUILDDIR}}/release/lcharmap"
	rm -f "{{BUILDDIR}}/debug/lcharmap"

dev-install:
	@echo "\tINSTALL\t\tbuild/debug/lcharmap"
	@install -m 755 "{{BUILDDIR}}/debug/lcharmap" "{{PREFIX}}{{DESTDIR}}/lcharmap"

install: dbgen release
	@echo "\tINSTALL\t\tbuild/release/lcharmap"
	@install -m 755 "{{BUILDDIR}}/release/lcharmap" "{{PREFIX}}{{DESTDIR}}/lcharmap"

uninstall:
	rm -f "{{PREFIX}}{{DESTDIR}}/lcharmap"

dbgen:
	@cd lib && just install

debug:
	@echo "\tRC\t\t{{CARGOPTS}}"
	@{{CARGOBIN}} {{CARGOPTS}}

release:
	@echo "\tRC\t\t{{CARGOPT_RELEASE}}"
	@{{CARGOBIN}} {{CARGOPT_RELEASE}}
