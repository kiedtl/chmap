#!/bin/sh
#
# Require GNU printf(3) and GNU bc

. tests/lib.sh

hex() {
    echo "obase=16;ibase=A;${1##0x}" | bc
}

decode() {
    tests/pilot_decode $(/usr/bin/printf "$1") >/dev/null
}

begin "Decode capital a"
cmdout decode '\x41'             'U+0041'

begin "Decode small o with diaeresis"
cmdout decode '\xc3\xb6'         'U+00F6'

begin "Decode cyrillic capital letter zhe"
cmdout decode '\xd0\x96'         'U+0416'

begin "Decode euro sign"
cmdout decode '\xe2\x82\xac'     'U+20AC'

begin "Decode musical symbol g cleff"
cmdout decode '\xf0\x9d\x84\x9e' 'U+1D11E'

begin "Fail on decoding surrogate chars"
cmdend decode '\xed\xa0\x82' 1

begin "Fail on first byte == 0xFF"
cmdend decode '\xff\xc3\xb1' 1

# The following tests were directly stolen from libutf8proc's
# test suite.

test_fail_missing_cont_byte() {
    for byte in $(seq 0xc0 0xff); do decode "\x$(hex $byte)" && return 1; done
    return 0
}
begin "Fail on missing continuation bytes"
test_fail_missing_cont_byte; chkend

test_fail_missing_cont_byte_before_noncont_0x41() {
    for byte in $(seq 0xc0 0xff); do decode "\x$(hex $byte)\x41" && return 1; done
    return 0
}
begin "Fail on lead followed by non-continuation byte 0x41"
test_fail_missing_cont_byte_before_noncont_0x41; chkend

test_fail_missing_cont_byte_before_noncont_0xc0() {
    for byte in $(seq 0xc0 0xff); do decode "\x$(hex $byte)\x41" && return 1; done
    return 0
}
begin "Fail on lead followed by non-continuation byte 0xc0"
test_fail_missing_cont_byte_before_noncont_0xc0; chkend

test_fail_lead_cont_byte() {
    for byte in $(seq 0x80 0xc3); do decode "\x$(hex $byte)" && return 1; done
    return 0
}
begin "Fail on leading continuation byte"
test_fail_lead_cont_byte; chkend

test_fail_overlong_2() {
    for byte in $(seq 0x81 0xbf); do
        decode "\xc0\x$(hex $byte)" && return 1
        decode "\xc1\x$(hex $byte)" && return 1
    done
    return 0
}
begin "Fail on overlong 2byte sequence"
test_fail_overlong_2; chkend

test_fail_overlong_3() {
    for byte in $(seq 0x80 0x9f); do
        decode "\xe0\x$(hex $byte)\x80" && return 1
    done
    return 0
}
begin "Fail on overlong 3byte sequence"
test_fail_overlong_3; chkend

test_fail_overlong_4() {
    for byte in $(seq 0x80 0x8f); do
        decode "\xf0\x$(hex $byte)\x80\x80" && return 1
    done
    return 0
}
begin "Fail on overlong 4byte sequence"
test_fail_overlong_4; chkend

test_fail_abovemax_4() {
    for byte in $(seq 0x90 0xbf); do decode "\xf4\x$(hex $byte)\x80\x80" && return 1; done
    for byte in $(seq 0xf5 0xf7); do decode "\x$(hex $byte)\x80\x80\x80" && return 1; done
    return 0
}
begin "Fail on 4byte sequence encoding > UNICODE_MAX"
test_fail_abovemax_4; chkend

test_fail_invalid_5() {
    for byte in $(seq 0xf8 0xfd); do
        decode "\xf7\x80\x80\x80\x$(hex $byte)" && return 1
    done
    return 0
}
begin "Fail on invalid 5byte sequence"
test_fail_invalid_5; chkend

test_fail_invalid_6() {
    for byte in $(seq 0xfc 0xfd); do
        decode "\xf7\x80\x80\x80\x80\x$(hex $byte)" && return 1
    done
    return 0
}
begin "Fail on invalid 6byte sequence"
test_fail_invalid_6; chkend
