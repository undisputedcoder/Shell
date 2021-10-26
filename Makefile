main: main.o token.o command.o
	gcc -Wall main.o token.o command.o -o main

token.o: token.h token.c
	gcc -c token.c 

main.o: main.c
	gcc -c main.c

command.o: command.c command.h
	gcc -c command.c

clean:
	rm *.o
