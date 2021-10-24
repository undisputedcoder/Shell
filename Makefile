main: main.o token.o
	gcc  -Wall main.o token.o -o main

token.o: token.h token.c
	gcc -c token.c 

main.o: main.c
	gcc -c main.c

clean:
	rm *.o
