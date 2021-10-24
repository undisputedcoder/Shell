#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <errno.h>

#include "token.h"
#include "command.h"

#include <unistd.h>

#define CHAR 30

int main() {

    /*Shell will not be terminated by CTRL-C, CTRL-Z or CTRL-\ */
    signal(SIGQUIT, SIG_IGN);
    signal(SIGINT, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);

    Command c[MAX_NUM_COMMANDS];
    char *t[MAX_NUM_TOKENS];
    char prompt[256] = "%";
    char buffer[256];
    char Exit[] = "exit";
    int nCommands; //number of commands in argument vector(argv)

    while(1) 
    {
    
    	//1)print the shell prompt
        printf("%s " ,prompt);
        
        //2) read a line using, fgets
        //Implementation found in: "Notes on Implementation of Shell Project"
	//Documentation author: Hong Xie
        int again = 1;
        char *linept; //pointer to line buffer

        while(again) 
        {
            again = 0;
            linept = fgets(buffer, 256, stdin);
            if(linept == NULL) {
                if(errno == EINTR) {
                    again = 1;  //signal intteruption, read again
                }
 
            }
        }

	//3) Analyse the command line
	//tokenise command line
        tokenise(buffer, t);
        
        //separate a list of token into a sequence of commands
        //returns number of commands
        nCommand=separateCommands(t, c);

        if(strcmp(buffer, Exit) != 0) {
            exit(0);
        }
          
        //4)Jobs in the command line
        for(int i=0; i < nCommands; ++i)
        {
        	//4.1) if the job is the exit command, then terminate the program
        	//4.2) create child processes (and pipes, redirections etc) 
        	//4.3) if the job is a background job (ie ended with &) continue
			//go back for loop
		//4.4)Sequential process, shell will wait for the job to finish
			//In this case until child has finished
        }
    }    

    return 0;
}
