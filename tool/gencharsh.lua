#!/usr/bin/env lua
--
-- (c) Kiëd Llaentenn <kiedtl@tilde.team>
-- See the COPYING file for copyright information.

function mytonumber(a, ...)
    if a and a ~= "" then
        return tonumber(a, ...)
    else
        return -1
    end
end

print("\
#include <stdint.h>\
#include \"unicode.h\"\
\
/* This file has automatically generated. */\
\
const struct CharInfo charinfos[UNICODE_MAX] = {\
")

local data = io.stdin:read('*all')
for line in data:gmatch("([^\n]+)\n?") do
    local ch, desc, category, bidi, decimal, olddesc, upper, lower = line:match("(.-);(.-);(.-);.-;(.-);.-;(.-);.-;.-;.-;(.-);.-;(.-);(.-);")

    ch = tonumber(ch, 16)
    category = "UC_" .. category
    bidi = "UBIDI_" .. bidi
    decimal = mytonumber(decimal)
    if desc == "<control>" then desc = olddesc end
    upper = mytonumber(upper, 16)
    lower = mytonumber(lower, 16)

    desc = desc:lower()

    print(string.format("\t[%5d] = { %s, %s, %d, \"%s\", %d, %d },",
        ch, category, bidi, decimal, desc, upper, lower))
end

print("};")
