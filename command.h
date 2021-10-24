#define MAX_NUM_COMMANDS  1000
#define MAX_ARGS 1000

// command separators
#define pipeSep  "|"                            // pipe separator "|"
#define conSep   "&"                            // concurrent execution separator "&"
#define seqSep   ";"                            // sequential execution separator ";"

struct CommandStruct {
   char *com_pathname;
   int argc;
   char *argv[MAX_ARGS];
   
   char *redirect_in;
   char *redirect_out;
   char com_suffix;
};

typedef struct CommandStruct Command;  // command type

int separateCommands(char *token[], Command command[]);

void buildCommandArgumentArray(char *token[], Command *cp);
