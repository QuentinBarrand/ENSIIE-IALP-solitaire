VERSION = -DVERSION=\"3.0\"
BUILD = -DBUILD=\"$(shell echo "obase=16; `date +%s`" | bc)\"

GCC_OPTS = -O3

.PHONY : doc

all: solitaire

solitaire: libsolitaire.a main.c
	gcc -o solitaire $(GCC_OPTS) $(VERSION) $(BUILD) main.c libsolitaire.a -lcurses

libsolitaire.a: src/Sjeu.o src/Sgui.o lib/coordutils.o lib/Stack.o
	ar -cr $@ src/Sjeu.o lib/coordutils.o src/Sgui.o lib/Stack.o

src/Sgui.o: src/Sgui.c src/Sgui.c
	gcc -c -o $@ $(GCC_OPTS) $(VERSION) $(BUILD) src/Sgui.c

src/Sjeu.o: src/Sjeu.h src/Sjeu.c
	gcc -c -o $@ $(GCC_OPTS) $(VERSION) $(BUILD) src/Sjeu.c

lib/coordutils.o: lib/coordutils.h lib/coordutils.c
	gcc -c -o $@ $(GCC_OPTS) $(VERSION) $(BUILD) lib/coordutils.c

lib/Stack.o: lib/Stack.c lib/Stack.h
	gcc -c -o $@ $(GCC_OPTS) $(VERSION) $(BUILD) lib/Stack.c

# You need a Makefile-formatted Makedoc file to call this target
doc:
	make -f Makedoc

dev:
	make GCC_OPTS="-Wall -Wextra -DDEBUG -g -O2"

clean:
	cd lib/; rm -f *.o
	cd src/; rm -f *.o
	rm *.a
	rm solitaire