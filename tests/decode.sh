#!/bin/sh
#
# Require GNU printf(3)

. tests/lib.sh

testdecode() {
    cmd$1 decode $(/usr/bin/printf '%b' "$2") $3
}

begin "Decode capital a"
testdecode out '\x41'             'U+0041'

begin "Decode small o with diaeresis"
testdecode out '\xc3\xb6'         'U+00F6'

begin "Decode cyrillic capital letter zhe"
testdecode out '\xd0\x96'         'U+0416'

begin "Decode euro sign"
testdecode out '\xe2\x82\xac'     'U+20AC'

begin "Decode musical symbol g cleff"
testdecode out '\xf0\x9d\x84\x9e' 'U+1D11E'

begin "Fail when decoding surrogate chars"
testdecode end '\xed\xa0\x82' 1

begin "Fail when first byte == 0xFF"
testdecode end '\xff\xc3\xb1' 1
