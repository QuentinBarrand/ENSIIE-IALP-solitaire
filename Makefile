GCC_OPTS = -Wall -Wextra -DDEBUG -g -O2

all: main.c libsolitaire.a
	gcc -o solitaire $(GCC_OPTS) main.c libsolitaire.a

libsolitaire.a: jeu.o util.o
	ar -cr $@ jeu.o util.o

jeu.o: jeu.c jeu.h
	gcc -c $(GCC_OPTS) jeu.c -o $@

util.o: util.c util.h
	gcc -c $(GCC_OPTS) util.c -o $@

clean:
	rm -rf *.a *.o