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
CC = cc
LD = gold

RELEASE_CFLAGS  = -Os
RELEASE_LDFLAGS = -march=native -flto -s
DEBUG_CFLAGS    = -O0 -ggdb
DEBUG_LDFLAGS   =
