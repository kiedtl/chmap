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

Tarballs are available from GitHub releases for Linux-x86_64 and
Linux-armv6l. Extract, `cd`, and run:

```
$ sudo ./install.sh /usr/local
```

### Building from Source

#### Build dependencies
- a C99 compiler, GNU Make
- [scdoc](https://git.sr.ht/~sircmpwn/scdoc) (manpage)

Download the latest source tarball from GitHub releases
(`chmap-v$VERSION.tar.xz`), extract, and build:

```
# make clean install
```

## How?

Run `chmap` with the `-r` flag (for *r*ange) to get info for a range of
Unicode runes:

```
$ chmap -r 0-5
codepoint  glyph  encoded      case   description
        0                      other  null
        1         1            other  start of heading
        2         2            other  start of text
        3         3            other  end of text
        4         4            other  end of transmission
        5         5            other  enquiry
```

Example ranges: `0-1`, `355-369`, `34`, `189-192,12,45-49`.

You may also list info for a list of given characters with the `-c` flag:

```
$ chmap -c Lovecraft
codepoint  glyph  encoded      case   description
       76  L      4C           upper  latin capital letter l
      111  o      6F           lower  latin small letter o
      118  v      76           lower  latin small letter v
      101  e      65           lower  latin small letter e
       99  c      63           lower  latin small letter c
      114  r      72           lower  latin small letter r
       97  a      61           lower  latin small letter a
      102  f      66           lower  latin small letter f
      116  t      74           lower  latin small letter t
```

You can search for characters that match a regex, too, with the `-s` flag:

```
$ chmap -s '^latin.*capital letter z'                                                       
codepoint  glyph  encoded      case   description
       90  Z      5A           upper  latin capital letter z
      377  Ź      C5 B9        upper  latin capital letter z with acute
      379  Ż      C5 BB        upper  latin capital letter z with dot above
      381  Ž      C5 BD        upper  latin capital letter z with caron
      437  Ƶ      C6 B5        upper  latin capital letter z with stroke
      548  Ȥ      C8 A4        upper  latin capital letter z with hook
     7824  Ẑ      E1 BA 90     upper  latin capital letter z with circumflex
     7826  Ẓ      E1 BA 92     upper  latin capital letter z with dot below
     7828  Ẕ      E1 BA 94     upper  latin capital letter z with line below
    11371  Ⱬ      E2 B1 AB     upper  latin capital letter z with descender
    11391  Ɀ      E2 B1 BF     upper  latin capital letter z with swash tail
    42950  Ᶎ      EA 9F 86     upper  latin capital letter z with palatal hook
```

Adding the `-l` flag causes `chmap` to print more information:

```
$ chmap -l -r 13000                                                                         
codepoint    13000 0x32C8  0o31310
encoding     UTF8(E3 8B 88)
glyph        ㋈ (2 columns)
description  ideographic telegraph symbol for september
case         other
category     Symbol (other)
```

See `man chmap` for more.

## Why?

I miss some Windows utilities.

I found [`chars`](https://github.com/antifuchs/chars), but it lacked
several features which I'd like:

- A table-like output mode (which `chmap` has by default). This makes
  viewing many entries more comfortable in a short terminal.
- A `category` field to specify the Unicode category that the rune falls
  under.

## Inspiration

- Window's `charmap.exe`
- Eric Raymond's `ascii` utility
- `chars` (https://github.com/antifuchs/chars)
- `uniname` from uniutils

## License

This lame little utility is licensed under the MIT License. See
the `LICENSE.md` file for more information.
