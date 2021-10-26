#include "token.h"

void tokenise(char line[], char *token[]) {
    char *tk;
    int i = 0;

    tk = strtok(line, tokens);
    token[i] = tk;

    while(tk != NULL) {
        ++i;
        if(i >= MAX_NUM_TOKENS) {
            i = -1;
            break;
        }

        tk = strtok(NULL, tokens);
        token[i] = tk;
    }

}

void setNullTokens(char* token[])
{
	for(int i=0;i<MAX_NUM_TOKENS;i++)
	{
		token[i]=NULL;
	}
}