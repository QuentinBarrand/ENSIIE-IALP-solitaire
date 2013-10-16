ifeq ($(SOLITAIRE_FLAG),DEV)
	GCC_OPTS = -Wall -Wextra -DDEBUG -g -O2
else
	GCC_OPTS = -O3
endif

.PHONY : doc

all: libsolitaire.a
	gcc -o solitaire $(GCC_OPTS) main.c lib/libsolitaire.a
	make clean

libsolitaire.a: jeu.o util.o
	cd lib/; ar -cr $@ jeu.o util.o

jeu.o:
	cd lib/; gcc -c -o $@ $(GCC_OPTS) jeu.c

util.o:
	cd lib/; gcc -c -o $@ $(GCC_OPTS) util.c

# You need a Makefile-formatted Makedoc file to call this target
doc:
	make -f Makedoc

dev:
	export SOLITAIRE_FLAG="DEV"
	make

clean:
	cd lib; rm -f *.a *.o