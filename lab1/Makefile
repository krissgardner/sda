# -*- Makefile -*-

all: app1 app2 app3

app1: app1.o bubblesort.o shakesort.o swap.o timer.o
	gcc app1.o bubblesort.o shakesort.o swap.o timer.o -o app1

app2: app2.o heapsort.o swap.o timer.o
	gcc app2.o heapsort.o swap.o timer.o -o app2

app3: app3.o fibonacci.o swap.o timer.o
	gcc app3.o fibonacci.o swap.o timer.o -o app3


# Dependency list
app1.o: app1.c algorithms.h timer.h
	gcc -c app1.c

app2.o: app2.c algorithms.h timer.h
	gcc -c app2.c

app3.o: app3.c algorithms.h timer.h
	gcc -c app3.c

bubblesort.o: bubblesort.c helpers.h
	gcc -c bubblesort.c

fibonacci.o: fibonacci.c
	gcc -c fibonacci.c

shakesort.o: shakesort.c  helpers.h
	gcc -c shakesort.c

heapsort.o: heapsort.c helpers.h
	gcc -c heapsort.c

swap.o: swap.c
	gcc -c swap.c

timer.o: timer.c timer.h
	gcc -c timer.c


# Clean
clean:
	rm *.o app1 app2 app3 stats*.txt