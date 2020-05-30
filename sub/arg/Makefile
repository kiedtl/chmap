# build argoat

include ../../config.mk

all: argoat.a

argoat.a: argoat.o
	@printf "    %-8s%s\n" "AR" $@
	$(CMD)$(AR) -rcs $@ $^

argoat.o: argoat.c
	@printf "    %-8s%s\n" "CC" $@
	$(CMD)$(CC) $(CFLAGS) $(RELEASE_CFLAGS) \
		-c $< -o $(<:.c=.o)

clean:
	$(CMD)rm -f *.a *.o

.PHONY: all clean
