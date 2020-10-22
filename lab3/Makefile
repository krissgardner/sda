# -*- Makefile -*-

all: p1 p2

# Executables
p1: p1.o
	gcc p1.o -o p1

p2: p2.o
	gcc p2.o -o p2

# Object files
p1.o: p1.c
	gcc -c p1.c

p2.o: p2.c
	gcc -c p2.c

clean:
	rm -f *.o p1 p2