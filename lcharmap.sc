lcharmap(1) "General Commands Manual"

# NAME

lcharmap - print and search information for Unicode characters.

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
	character(s). (e.g., `Kiëd Llaentenn')

*-s, --search* REGEX
	Search character descriptions for *term*.

# EXAMPLES

lcharmap --range 128,136\/\fR

lcharmap --chars ThisIsATest

lcharmap --search "^LATIN (SMALL|CAPITAL) A"

# KNOWN ISSUES

- The source code is extremely messy and must be cleaned up.

- The resulting binary isn't as small as I would like it to
be (debug: 12M, release: 1.4M).

- searching is very slow, due to large database size.

# REPORTING BUGS

Please report any found bugs to https://github.com/lptstr/lcharmap/issues
