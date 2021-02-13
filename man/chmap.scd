chmap(1) "General Commands Manual"

# NAME

chmap - Get information for unicode characters

# SYNOPSIS

*chmap* [-rcs] [ARGS]...

# DESCRIPTION

chmap (formerly lcharmap) retrieves information regarding Unicode
characters, including a short description, its UTF8 encoding, the Unicode
category it belongs to, its casing, and its decimal/hexadecimal/octal
representation.

# OPTIONS

*-h*
	Print a short help message and exit.

*-V*
	Print chmap's version and exit.
*-l*
	Print information in the long format.

# COMMANDS

*-r* _RANGE_
	Print a range of Unicode codepoints. (e.g. `0x033-0x035',
	or `1,3,6-9')

	Range values can be in binary, octal, hexadecimal, in addition
	to decimal. In that case, they must be prefixed with a `0b', `0o',
	or `0x`, respectively.

*-c* _CHARS_
	Print a range of Unicode codepoints that match the provided
	character(s). (e.g., `Kiëd')

*-s* _REGEX_
	Search character descriptions for _REGEX_.

# EXAMPLES

chmap -r 33,36,45-47
	Print information for characters 33, 36, 45, 46, and 47.

chmap -r 0o033,0xAB,0b1101
	Print information for characters 27, 171, and 13.

chmap -c Asbjørn
	Print information for all characters in the name `Asbjørn'.

chmap -s "^latin .\* a$"
	Search with regular expression "^latin .\* a$".

# KNOWN ISSUES

- chmap is currently buggy and untested on Windows and BSD.
- The display code is utterly incomprehensible.

# AUTHORS

Kiëd Llaentenn <kiedtl@tilde.team>

# REPORTING BUGS

Send bugs reports, hate mail, and other chmap-related bikeshedding to the
author's email above, or */msg kiedtl* on freenode.

# SEE ALSO

The full documentation for *chmap* is not maintained as a Texinfo manual.
If the *info* and *chmap* programs are properly installed on your system,
the command

	*info chmap*

should not give you access to the complete manual.
