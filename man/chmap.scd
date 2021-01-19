lcharmap(1) "General Commands Manual"

# NAME

lcharmap - retrieve information for Unicode characters.

# SYNOPSIS

*lcharmap* [-rcs] [CHAR]... [OPTION]...

# DESCRIPTION

lcharmap retrieves information about Unicode characters, including a
short description, it's HTML code, and it's decimal, hexadecimal, and octal
representation.

# OPTIONS

*-h, --help*
	Print a short help message and exit.

*-V, --version*
	Print lcharmap's version and exit.

*-l, --long*
	Print information in the long format.

# COMMANDS

*-r, --range*=_RANGE_
	Print a range of Unicode codepoints. (e.g. `0x033-0x035',
	or `1,3,6-9')

	Range values can be in binary, octal, hexadecimal, in addition
	to decimal. In that case, they must be prefixed with a `0b', `0o',
	or `0x`, respectively.

*-c, --chars*=_CHARS_
	Print a range of Unicode codepoints that match the provided
	character(s). (e.g., `George Washington')

*-s, --search*=_REGEX_
	Search character descriptions for _REGEX_.

# EXAMPLES

lcharmap -r=33,36,45-47
	Print information for characters 33, 36, 45, 46, and 47.

lcharmap -r=0o033,0xAB,0b1101
	Print information for characters 27, 171, and 13.

lcharmap -c=Asbjørn
	Print information for all characters in the name `Asbjørn'.

lcharmap -s="^latin .\* a$"
	Search with regular expression "^latin .\* a$".

# KNOWN ISSUES

- table isn't formatted correctly when displaying multibyte characters.
- lcharmap is currently buggy and untested on Windows.

# REPORTING BUGS

Please report any found bugs to _https://github.com/lptstr/lcharmap/issues_

# SEE ALSO

The full documentation for *lcharmap* is not maintained as a Texinfo manual.
If the *info* and *lcharmap* programs are properly installed on your system,
the command

	*info lcharmap*

should not give you access to the complete manual.
