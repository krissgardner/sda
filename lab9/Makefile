p1: p1.c
	gcc -Wall -o p1 p1.c

check: p1 
	valgrind -v --leak-check=full ./p1

clean:
	rm -rf p1