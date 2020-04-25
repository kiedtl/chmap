# TODO

## v2.1.0 -- new fields, misc

- new field: binary representation
- new field: UTF-8 encoding
- `--chars`: accept input from stdin
- only links bits of sqlite3 that are actually needed
- use "real" tables library instead of custom one (for long format)

## v2.2.0 -- Winbl^H^Hdows support

- use cross-platform regex library instead of `regex.h` (PCRE?)
- complete Windows support for terminfo.c, dirs.c
- move terminfo.c, dirs.c into separate submodules

## v2.3.0

- throw tantrum on invalid UTF-8 input
- `--range`: support number characters from other languages

---

**other:** `grep -nRE 'XXX|TODO' src`
