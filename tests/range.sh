#!/bin/sh

. ./lib.sh

begin "Lone stranger"
cmdout ./pilot_range "128"   "128"

begin "Simple start-to-end"
cmdout ./pilot_range "0-5"   "0 1 2 3 4 5"

begin "Start-to-end with stranger"
cmdout ./pilot_range "0-5,8" "0 1 2 3 4 5 8"

begin "Start-to-end with hexadecimal stranger"
cmdout ./pilot_range "0-5,0xFF" "0 1 2 3 4 5 255"

begin "Strangers with negative values"
cmdout ./pilot_range "12,-9,-5--2" "12 -9 -5 -4 -3 -2"

begin "Fail when end < start"
cmdend ./pilot_range "5-2" 1

begin "Fail on invalid number"
cmdend ./pilot_range "0-abcd" 1

begin "Fail on invalid hexadecimal number"
cmdend ./pilot_range "0-12,24,0xFZ" 1

begin "Fail when missing end of start-to-end"
cmdend ./pilot_range "12,4-9,0-" 1

begin "Fail when missing start of start-to-end"
cmdend ./pilot_range "12,--9,0-8" 1
