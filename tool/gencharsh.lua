#!/usr/bin/env lua
--
-- (c) Kiëd Llaentenn <kiedtl@tilde.team>
-- See the COPYING file for copyright information.

print("#ifndef CHARS_H")
print("#define CHARS_H")

print("")
print("/* This file has automatically generated. */")
print("")
print("#define UNICODE_MAX 0x10FFFF")
print("")
print("struct CharInfo {")
print("\tchar *desc;")
print("} charinfos[UNICODE_MAX] = {")

local data = io.stdin:read('*all')
for line in data:gmatch("([^\n]+)\n?") do
    local ch, desc, edesc = line:match("(.-);(.-);.-;.-;.-;.-;.-;.-;.-;.-;(.-);")

    ch = tonumber(ch, 16)
    if desc == "<control>" then desc = edesc end

    desc = desc:lower()

    print(string.format("\t[%5d] = { \"%s\" },", ch, desc))
end

print("};")
print("#endif")
