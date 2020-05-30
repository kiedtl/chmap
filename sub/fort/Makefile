# build libfort

include ../../config.mk

all: fort.a

fort.a: fort.o
	@printf "    %-8s%s\n" "AR" $@
	$(CMD)$(AR) -rcs $@ $^

fort.o: fort.c
	@printf "    %-8s%s\n" "CC" $@
	$(CMD)$(CC) $(CFLAGS) $(RELEASE_CFLAGS) \
		-c $< -o $(<:.c=.o)

clean:
	$(CMD)rm -f *.a *.o

.PHONY: all clean
