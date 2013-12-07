VERSION = -DVERSION=\"3.0\"
BUILD = -DBUILD=\"$(shell echo "obase=16; `date +%s`" | bc)\"

GCC_OPTS = -O3

.PHONY : doc

all: solitaire

solitaire: lib/libsolitaire.a main.c
	gcc -o solitaire $(GCC_OPTS) $(VERSION) $(BUILD) main.c lib/libsolitaire.a -lcurses

lib/libsolitaire.a: lib/Sjeu.o lib/Scoordutils.o lib/Sgui.o lib/stack.o
	ar -cr $@ lib/Sjeu.o lib/Scoordutils.o lib/Sgui.o lib/stack.o

lib/Scoordutils.o: lib/Scoordutils.h lib/Scoordutils.c
	gcc -c -o $@ $(GCC_OPTS) $(VERSION) $(BUILD) lib/Scoordutils.c

lib/Sgui.o: lib/Sgui.c lib/Sgui.c
	gcc -c -o $@ $(GCC_OPTS) $(VERSION) $(BUILD) lib/Sgui.c

lib/Sjeu.o: lib/Sjeu.h lib/Sjeu.c
	gcc -c -o $@ $(GCC_OPTS) $(VERSION) $(BUILD) lib/Sjeu.c

# You need a Makefile-formatted Makedoc file to call this target
doc:
	make -f Makedoc

dev:
	make GCC_OPTS="-Wall -Wextra -DDEBUG -g -O2"

clean:
	cd lib/; rm -f *.a *.o
	rm solitaire