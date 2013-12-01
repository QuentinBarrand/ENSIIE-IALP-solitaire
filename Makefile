VERSION = 3.0
BUILD = $(shell date +%s)

GCC_OPTS = -O3 -DVERSION=$(VERSION) -DBUILD=$(BUILD)

.PHONY : doc

all: solitaire

solitaire: lib/libsolitaire.a main.c
	gcc -o solitaire $(GCC_OPTS) main.c lib/libsolitaire.a

lib/libsolitaire.a: lib/Sjeu.o lib/Sutils.o
	ar -cr $@ lib/Sjeu.o lib/Sutils.o

lib/Sjeu.o: lib/Sjeu.h lib/Sjeu.c
	gcc -c -o $@ $(GCC_OPTS) lib/Sjeu.c

lib/Sutils.o: lib/Sutils.h lib/Sutils.c
	gcc -c -o $@ $(GCC_OPTS) lib/Sutils.c

# You need a Makefile-formatted Makedoc file to call this target
doc:
	make -f Makedoc

dev:
	make GCC_OPTS="-Wall -Wextra -DDEBUG -g -O2 -DVERSION=$(VERSION) -DBUILD=$(BUILD)"

clean:
	cd lib/; rm -f *.a *.o
	rm solitaire