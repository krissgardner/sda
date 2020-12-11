
app1: aplicatia1.c
	gcc -Wall -o app1 aplicatia1.c

check: app1 
	valgrind -v --leak-check=full ./app1

clean:
	rm -rf app1