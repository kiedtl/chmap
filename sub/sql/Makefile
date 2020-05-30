# build sqlite

include ../../config.mk

all: sqlite3.a

sqlite3.a: sqlite3.o
	@printf "    %-8s%s\n" "AR" $@
	$(CMD)$(AR) -rcs $@ $^

sqlite3.o: sqlite3.c
	@printf "    %-8s%s\n" "CC" $@
	$(CMD)$(CC) $(CFLAGS) $(RELEASE_CFLAGS) \
		-c $< -o $(<:.c=.o)

clean:
	$(CMD)rm -f *.a *.o

.PHONY: all clean
