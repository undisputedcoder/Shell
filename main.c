#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <errno.h>

#include "token.h"
#include "command.h"

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

    while(1) {
        printf("%s " ,prompt);
        
        int again = 1;
        char *linept; //pointer to line buffer

        while(again) {
            again = 0;
            linept = fgets(buffer, 256, stdin);
            if(linept == NULL) {
                if(errno == EINTR) {
                    again = 1;  //signal intteruption, read again
                }
 
            }
        }

        //tokenise(buffer, t);

        if(strcmp(buffer, Exit) != 0) {
            exit(0);
        }
    }    

    return 0;
}