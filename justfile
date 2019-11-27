#
# lcharmap: a Linux port of the Windows `charmap` utility. 
# https://github.com/lptstr/lcharmap
#

# ----- VARIABLES -----

BUILDDIR  := "build"
PLATFORM  := `gcc -dumpmachine`

CARGOPTS := "build -j`nproc` --target-dir " + BUILDDIR
CARGOBIN := 'cargo'

CARGOPT_RELEASE := CARGOPTS + " --release"

PREFIX := '/usr'
DESTDIR := '/bin'

# ----- RECIPES -----
all: options debug

clean:
	rm -f "{{BUILDDIR}}/release/lcharmap"
	rm -f "{{BUILDDIR}}/debug/lcharmap"

options:
	@echo "OPTIONS:"
	@echo "\tRC\t\t\t= {{CARGOBIN}}"
	@echo "\tRCFLAGS\t\t\t= {{CARGOPTS}}"
	@echo "\tBUILDDIR\t\t\t= {{BUILDDIR}}"
	@echo "\tRCFLAGS_RELEASE\t\t= {{CARGOPT_RELEASE}}"
	@echo "\tPLATFORM\t\t= {{PLATFORM}}"
	@echo ""

dev-install:
	install -m 755 "{{BUILDDIR}}/debug/lcharmap" "{{PREFIX}}{{DESTDIR}}/lcharmap"

install:
	install -m 755 "{{BUILDDIR}}/release/lcharmap" "{{PREFIX}}{{DESTDIR}}/lcharmap"

uninstall:
	rm -f "{{PREFIX}}{{DESTDIR}}/lcharmap"

debug: options
	@echo "RC {{CARGOPTS}}"
	@{{CARGOBIN}} {{CARGOPTS}}

release: options
	@echo "RC {{CARGOPT_RELEASE}}"
	@{{CARGOBIN}} {{CARGOPT_RELEASE}}
