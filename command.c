#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "command.h"
	
// return 1 if the token is a command separator
// return 0 otherwise
// author: Hong Xie, from week 9 lab
int separator(char *token)
{
    int i=0;
    char *commandSeparators[] = {pipeSep, conSep, seqSep, NULL};

    while (commandSeparators[i] != NULL) {
        if (strcmp(commandSeparators[i], token) == 0) {
            return 1; 
        } 
        ++i;
    }
    
    return 0;
}

// fill command structure of argc 
// fill command suffix with the details
void fillCommandStructure(Command *cp, int first, int last, char *com_suffix)
{   
     cp->argc = first-last; //Number of command arguments count
     cp->com_suffix=*com_suffix; //Command suffix: ; & |
}


// process standard in/out(</>) redirections in a command
// If a redirect if found set redirect to that token and int count will increase
void searchRedirection(char *token[], Command *cp, int *count) 
{
    int i = 0;
    while(i < cp->argc) 
    {
    	//standard input("<") redirection
        if(strcmp(token[count], "<") == 0) 
        {  
            cp->redirect_in = token[*count+1]; // '<' found therefore increase count by 1
        } 
        //standard output(">") redirection
        if (strcmp(token[count], ">") == 0) 
        { 
            cp->redirect_out = token[*count+1]; // '>' found therefore increase count by 1
        }
        (*count)++;
        ++i;
    } 
    (*count)++;
}

// build command line argv (argument vector) for execvp function
void buildCommandArgumentArray(char *token[], Command *cp, int *count) 
{
    //reduce argument count by 2 if either redirection is found
    if(cp->redirect_in!=NULL || cp -> redirect_out!=NULL)
    {
	cp->argc -= 2;
    }
    
    if(cp->argv == NULL) 
    {
        perror("ERROR: No command found");
        exit(1);
    }

    int i;
    int k = 0;
    
    //Sets command path name to token[0]
    cp->com_pathname=token[*count]
    
    //Loops around for number of command arguments 
    //sets command arguement vector array
    for(i = 0; i < cp->argc; (*count)++)
    {	
        cp -> argv[k] = token[*count];
	++k;
	++i;
		
    }
    (*count)++;  
    cp->argv[k]=NULL;

    //increase int count by 2 if either redirection is found
    //so the next command arugment array arrangement is in the right destination
    if(cp->redirect_in != NULL || cp->redirect_out != NULL)
    {
        (*count)+=2;
    }

}

//
//fill command line argument vector
//used some emplementation from Author: Hong Xie, from week 9 lab
int separateCommands(char *token[], Command command[])
{
     int i;
     int nTokens;

     // find out the number of tokens
     i = 0;
     while (token[i] != NULL) ++i; 
     nTokens = i;

     // if empty command line
     if (nTokens == 0) 
          return 0;

     // check the first token
     if (separator(token[0]))
          return -3;

     // check last token, add ";" if necessary 
     if (!separator(token[nTokens-1])) 
     {
          token[nTokens] = seqSep;
          ++nTokens;
     }
          
     int first=0;   // points to the first tokens of a command
     int last;      // points to the last  tokens of a command
     char *sep;     // command separator at the end of a command
     int c = 0;         // command index
     
     for (i=0; i<nTokens; ++i) 
     {
         last = i;
         if (separator(token[i]))
          {
             sep = token[i];
             if (first==last)  // two consecutive separators
                 return -2;
             fillCommandStructure(&(command[c]), first, last, sep);
             ++c;
             first = i+1; 
         }
     }

     // check the last token of the last command  
     if (strcmp(token[last], pipeSep) == 0)
     { // last token is pipe separator
          return -4; 
     } 
     
     //calculate the number of commands
     int nCommands = c;
     
     //counts redirection using token
     int *countRedirection=0;
     
     //counts argument array using token
     int *countArgumentArray=0;
     
     //loops through number of commands
     //to fill redirection and argv for each command
     for(i=0; i < nCommands; ++i) {
        searchRedirection(token, &command[i], countRedirection);
        buildCommandArgumentArray(token, &command[i], countArgumentArray);
     }
 
     return nCommands; 
}

//Test parser for debugging
//Implementation found in: "Notes on Implementation of Shell Project"
//Documentation author: Hong Xie
void printComStruct(Command *com)
{ 
	int i;

	fprintf(stderr,"com_pathname=%s\n", com->com_pathname); 
	fprintf(stderr,"argc=%d\n", com->argc); 
	for(i=0; i<com->argc; i++)
	{
		fprintf(stderr,"argv[%d]=%s\n", i, com->argv[i]);
	}
	fprintf(stderr,"#######\n"); 
	if (com->redirect_in == NULL)
	{
		fprintf(stderr,"redirect_in=NULL\n"); 
	}
	else
	{
		fprintf(stderr,"redirect_in=%s\n", com->redirect_in); 
	}
	if (com->redirect_out == NULL)
	{
		fprintf(stderr,"redirect_out=NULL\n"); 
	}
	else
	{
		fprintf(stderr,"redirect_out=%s\n", com->redirect_out); 
	}
	fprintf(stderr,"com_suffix=%c\n\n", com->com_suffix);
}

