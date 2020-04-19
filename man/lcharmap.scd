lcharmap(1) "General Commands Manual"

# NAME

lcharmap - retrieve information for Unicode characters.

# SYNOPSIS

*lcharmap* [OPTIONS] [ARGUMENTS]

# DESCRIPTION

lcharmap prints information for the provided character codes, including decimal,
hexadecimal, octal, and HTML codes. It also prints out a description of each
character, taken from it's character database located at
_$HOME/.local/share/lcharmap/chars.db_ (on Linux).
This character database is generated automatically on installation from
the UCD (Unicode Character Database).

# OPTIONS

*-h, --help*
	Print a short help message and exit.

*-V, --version*
	Print lcharmap's version and exit.

*-l, --long*
	Print information in the long format.

# ARGUMENTS

*-r, --range* START,END
	Print a range of Unicode codepoints. (e.g., `128,255')

*-c, --chars* CHARS
	Print a range of Unicode codepoints that match the provided
	character(s). (e.g., `George Washington')

*-s, --search* REGEX
	Search character descriptions for *REGEX*.

# EXAMPLES

lcharmap --range 128,136

lcharmap --chars ThisIsATest

lcharmap --search "^LATIN (SMALL|CAPITAL) A"

# KNOWN ISSUES

- lcharmap currently only supports English.
- lcharmap is currently buggy and untested on Windows.

# REPORTING BUGS

Please report any found bugs to _https://github.com/lptstr/lcharmap/issues_
