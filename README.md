# `chmap` (formerly `lcharmap`)

> A CLI utility to get information for Unicode characters.

## What?

`chmap` is a little utility to get information on Unicode characters,
such as its description, hexadecimal/octal representation, Unicode
category, UTF-8 encoding, and more.

```
$ chmap -r 9-10,13935,255-258
codepoint  glyph  encoded     case   description
        9         9           other  character tabulation
       10         A           other  line feed (lf)
    13935  㙯     E3 99 AF    other  modifier letter chinese tone yin ping
      255  ÿ      C3 BF       lower  latin small letter y with diaeresis
      256  Ā      C4 80       upper  latin capital letter a with macron
      257  ā      C4 81       lower  latin small letter a with macron
      258  Ă      C4 82       upper  latin capital letter a with breve
```

`chmap` was inspired by the `charmap.exe` tool present in Windows XP and
later.

## Where?
### Binaries
In the future, compiled binaries for the following platforms will be
available to download from GitHub Releases:
- Linux (`x86_64 glibc`)
- Linux (`x86_64 musl`)
- Linux (`armv6l glibc`)
- Linux (`armv6l musl`)
- Windows (`x86_64`)
- BSD (`x86_64`)

For now, you must build from source.

### Platforms
- macOS (*not supported or tested*)
- Windows (*not supported (yet), not tested*)
- Free|Open|Net|Dragonfly BSD (*not supported (yet), not tested*)
- Linux (*supported and tested*)

### Building from Source

#### Build dependencies
- a C99 compiler, GNU Make
- [scdoc](https://git.sr.ht/~sircmpwn/scdoc) (manpage)
- SQLite3, utf8proc

Download the latest source tarball from GitHub releases
(`chmap-v$VERSION.tar.xz`), extract, and build:

```
# make install
```

## How?

See `man chmap`.

## Why?

I miss some Windows utilities.

## Inspiration

- Window's `charmap.exe`
- Eric Raymond's `ascii` utility
- `chars` (https://github.com/antifuchs/chars)
- `uniname` from uniutils

## License

This lame little utility is licensed under the MIT License. See
the `LICENSE.md` file for more information.
