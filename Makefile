GCC_OPTS = -Wall -Wextra -DDEBUG -g -O2

.PHONY : doc

all: main.c libsolitaire.a
	gcc -o solitaire $(GCC_OPTS) main.c libsolitaire.a
	make clean

libsolitaire.a: jeu.o util.o
	ar -cr $@ jeu.o util.o

jeu.o: jeu.c jeu.h
	gcc -c $(GCC_OPTS) jeu.c -o $@

util.o: util.c util.h
	gcc -c $(GCC_OPTS) util.c -o $@

# You need a Makefile-formatted Makedoc file to call his target
doc:
	make -f Makedoc

clean:
	rm -rf *.a *.o