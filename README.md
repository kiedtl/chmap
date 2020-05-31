# `lcharmap`

> Get information on Unicode characters in the terminal

### Status

A few bugs remain to be fixed, but the tool is about 70% complete at this
point. Check [`man/TODO.md`](man/TODO.md) to see whats planned for later
releases.

### What?

`lcharmap` is a little utility to get information on Unicode characters,
including its name and hexadecimal/octal representation. It is also capable
of searching the UCD (Unicode Character Database) with a regular
expression.

`lcharmap` was inspired by the `charmap.exe` tool present in Windows XP and
newer.

### Where?
#### Platforms
- macOS (*not supported or tested*)
- Linux (*supported and tested*)
- Windows (*not supported (yet), not tested*)
- Free|Open|Net|Dragonfly BSD (*not supported (yet), not tested*)

#### Build dependencies
- a C99 compiler
- GNU Make
- `ar` 

#### Binaries
In the future, compiled binaries for the following platforms will be
available to download from GitHub Releases:
- Linux (`x86_64 glibc`)
- Linux (`x86_64 musl`)
- Linux (`armv6l glibc`)
- Linux (`armv6l musl`)
- Windows (`x86_64`)
- BSD (`x86_64`)

For now, you must build from source.

#### Building from Source

Download the latest source tarball from GitHub releases
(`lcharmap-v$VERSION.tar.xz`), extract, and build:

```
# make install
```

**Note:** `make install` also installs the character database, so if you
install manually, ensure that you complete that step with `make -C lib`.

### How?

Simply run `lcharmap -r` with a range of characters you want info for:
```
$ lcharmap -r 65,66
--------------------------------------------------------------------
DEC	HEX	OCT	HTML	CHAR    DESC
--------------------------------------------------------------------
65	41	101	&#41;	A       LATIN CAPITAL LETTER A
--------------------------------------------------------------------
66	42	102	&#42;	B       LATIN CAPITAL LETTER B
--------------------------------------------------------------------
```
You can run it on a single code point (or character!) too:
```
$ lcharmap -r 67
               decimal  67
           hexadecimal  43
                 octal  103
           HTML entity  &#43;
             character  C
           description  LATIN CAPITAL LETTER C

$ lcharmap -c C
               decimal  67
           hexadecimal  43
                 octal  103
           HTML entity  &#43;
             character  C
           description  LATIN CAPITAL LETTER C
```

**Note:** if you wish, you can run `lcharmap --chars` on multiple characters as well (e.g. `lcharmap -c ThisIsATest`)

Searching for a character is easy, too:
```
$ lcharmap -s "^LATIN (SMALL|CAPITAL) LETTER A$"
```

Run `lcharmap --help`/`man lcharmap` for more information.

### Why?
I miss some Windows utilities.

### Inspiration

- Window's `charmap.exe`
- Eric Raymond's `ascii` utility
- `uniname` from uniutils

### License

This lame little utility is licensed under the MIT License. See
the `LICENSE.md` file for more information.
