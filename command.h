#define MAX_NUM_COMMANDS  100
#define MAX_ARGS 1000

// command separators
#define pipeSep  "|"                            // pipe separator "|"
#define conSep   "&"                            // concurrent execution separator "&"
#define seqSep   ";"                            // sequential execution separator ";"

struct CommandStruct { 
     char *com_pathname;     // what is the path name of the command
     int argc;               // the number of arguments to the command
                             // including the command itself
     char *argv[MAX_ARGS];   // pointers to strings, each string
                             // is an argument for the command, including
                             // argument "0". The last pointer should
                             // be set to NULL. 
     char *redirect_in;      // if this is not NULL, then the standard input
                             // is redirected to the given file name
     char *redirect_out;     // if this is not NULL, then the standard output
                             // is redirected to the given file name
     char com_suffix;        // ' ' - no command suffix (last command);
                             // '&' - the command is followed by '&'
                             // ';' - the command is followed by ';';
                             // '|' - the command is followed by '|'.
};

typedef struct CommandStruct Command;  // command type

int separateCommands(char *token[], Command command[]);

void buildCommandArgumentArray(char *token[], Command *cp, int *count);

void printComStruct(Command *command);

void setNullCommands(Command commands[]);