main: main.o token.o command.o
	gcc -Wall main.o token.o command.o -o main

main.o: main.c 
	gcc -c main.c

token.o: token.c token.h
	gcc -c token.c 

command.o: command.c command.h
	gcc -c command.c

clean:
	rm *.o


	
