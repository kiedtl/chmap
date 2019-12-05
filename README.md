# `lcharmap`

> A CLI port of the Windows `charmap` utility

### What?

`lcharmap` is a CLI port of the Windows `charmap.exe` utility for Unix.
It displays information for a particular Unicode code point, including
it's hexadecimal/octal and HTML entity representation. 

In addition, it dislays character description from an extremely bloated
character database (2.8M) and allows you to search for character based
on their description (provided, of course, you are willing to wait while
lcharmap load the entire database into memory and matched each entry).

### Where?
#### Dependencies
- Windows (*not supported, not tested*)
- macOS (*not supported or tested*)
- Linux (*supported and tested*)
- Free|Open|Net|Dragonfly BSD (*not supported, not tested*)

#### Build dependencies
- the GNU C compiler (`gcc`)
- `binutils`
- the Rust compiler toolchain (Stable)
- [just](https://github.com/casey/just) 

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
Clone the project:
```
$ git clone https://github.com/lptstr/lcharmap
```

Build:
```
$ cd lcharmap
$ just release
```

Install
```
# just install
```

*Note: on the installation step, you may need root/admin privileges.*
*Note: `just install` also installs the character database to `/etc/chars.db`, so if 
you install manually, ensure that you complete that step with `cd lib; just install`.**

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

### Known Issues
- the source is extremely messy.
- resulting binary isn't as small as I would like it to be (debug: 12M, release: ?M)
- very slow on certain systems (due to the fact that `lcharmap` loads a *2.8* database to search. there doesn't
seem to be any way around this, see below)

#### the description database
Currently, lcharmap uses a database generated with upddb (located in `lcharmap/lib/upddb.c`) in `/etc/chars.db`. The database contains the
descriptions for each character. Each database entry is exactly 85 bytes long, which has two advanteges:
- for loading the description for a single codepoint, we can simply move the file pointer to where we know the description is 
  read 85 bytes. No parsing or lexing needed, and there is no need to load the entire 3M database at once.
- for loading the description for multiple codepoints, we just do the above multiple times. No parsing or lexing needed here either.
Unfortunately:
- for searching, we must load the entire database at once, read every description, and check if the provided term matches it.
  if someone has a better idea of how to do this without loading the entire database at once, please file an issue.
- most of the descriptions *aren't* 85 bytes long. This means that when the database is generated, "filler bytes" (of the value `0x00`)
  have to be added to the end of the description to make it the same size. Which means that the database is 3 time bigger that it could
  be (2.8M instead of 800K). In the future this will be fixed by storing a byte count in the first byte of each description, thus negating
  the need for each description to be the same size (since we will already know how long each description is simply from reading the first byte).

### License
This lame little utility is licensed under the MIT License. See the `LICENSE.md`
file for more information.
