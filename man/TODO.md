# TODO

## v1.1.0 -- new fields, misc

- new field: binary representation
- new field: UTF-8 encoding
- `--chars`: accept input from stdin
- only links bits of sqlite3 that are actually needed

## v1.2.0 -- Winbl^H^Hdows support

- use cross-platform regex library instead of `regex.h` (PCRE?)
- complete Windows support for terminfo.c, dirs.c
- move terminfo.c, dirs.c into separate submodules

## v1.3.0

- throw tantrum on invalid UTF-8 input
- `--range`: support number characters from other languages
- remove long format entirely
- add ability to disable/enable individual fields from table from cmd args
---

**other:** `grep -nRE 'XXX|TODO' src`
