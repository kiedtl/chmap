# TODO

Sorted in order of the chance that they'll be implemented in the next 10
years.

- Upgrade to Unicode 14
- "--pager" flag
- "-e" command to search emojis by CLDR (see https://github.com/arp242/uni)
  - "--tone" flag to change skin tone
  - "--gender" flag to change gender
- "-g" to print/search from Unicode groups
- Move "-s" command to "--search"/"--filter" flag, so that it can be
  combined with other commands
  - Search modifiers: -or
- new field: UTF-16 encoding
- -c should sort and deduplicate input
- an -f flag to filter output by property (e.g. `-f case=upper`)
- simplify display code
- X11/ncurses client, which an interface similar to charmap.exe
- use cross-platform regex library instead of `regex.h` (PCRE?)

---

**other:** `grep -nRE 'FIXME|XXX|TODO' src`
