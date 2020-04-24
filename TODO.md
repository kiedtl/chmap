# TODO

- `--range`: support number characters from other languages
- rename ccommon to libutil, move ecalloc to libutil
- update documentation (help message, manpage)
- use "real" tables library instead of custom one (for long format)

## new fields

- binary representation
- UTF-8 encoding

## Winbl^H^Hdows support

- use cross-platform regex library instead of `regex.h` (PCRE?)
- complete Windows support for terminfo.c, dirs.c
- move terminfo.c, dirs.c into separate submodules

**other:** `grep -nRE 'XXX|TODO' src`
