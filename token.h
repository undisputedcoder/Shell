#include <string.h>
#include <stdio.h>

#define MAX_NUM_TOKENS 100
#define tokens " \t\n"

void tokenise (char * inputLine, char *token[]);
void setNullTokens(char *[]);
