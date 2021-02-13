#
# lcharmap: retrieve information about Unicode characters
# https://github.com/lptstr/lcharmap
#
# (c) Kiëd Llaentenn and contributors
# See the LICENSE.md file for more information
#

CMD = @

DESTDIR =
PREFIX  = /usr/local

AR = ar
CC = clang
LD = bfd

RELEASE_CFLAGS  = -O3 -mtune=native -march=native
RELEASE_LDFLAGS = -flto -s
DEBUG_CFLAGS    = -Og -g
DEBUG_LDFLAGS   =
