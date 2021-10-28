#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <errno.h>

#include "token.h"
#include "command.h"

#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>

int main() {

    /*Shell will not be terminated by CTRL-C, CTRL-Z or CTRL-\ */
    signal(SIGQUIT, SIG_IGN);
    signal(SIGINT, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);

    Command c[MAX_NUM_COMMANDS];
    char *t[MAX_NUM_TOKENS];
    char prompt[256] = "%";
    char buffer[256];
    int nCommands = 0; //number of commands in argument vector(argv)
    pid_t pid;

    //Implementation found in: "Notes on Implementation of Shell Project", author: Hong Xie
    while(1) 
    {
        setNullTokens(t);
        setNullCommands(c);
        
        int again = 1;
        char *linept = NULL; //pointer to line buffer

    	//1)print the shell prompt
        printf("%s " ,prompt);
        
        //2) read a line using, fgets
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
        nCommands = separateCommands(t, c);
          
        //4)Jobs in the command line
        for(int i=0; i < nCommands; ++i)
        {
            //4.1) if the job is the exit command, then terminate the program
            if(strcmp(c[i].argv[0], "exit") == 0) {
                exit(0);
            }

            else if(strcmp(c[i].argv[0], "prompt") ==0) {
                //check if string exceeds size
                strcpy(prompt, c[i].argv[1]);
                continue;
            }

            else if(strcmp(c[i].argv[0], "pwd") == 0) {
                char pwd[256];
                getcwd(pwd, 256);
                printf("CURRENT DIRECTORY: %s\n", pwd);
                continue;
            }

            //Directory walk using command 'cd'
			else if(strcmp(c[i].argv[0], "cd")==0)
			{
				//if user typed '..' this allow them to go back to previous directory 
				if(c[i].argc == 2)
				{
					chdir(c[i].argv[1]);
					continue;
				}
				//set current directory to home if user type 'cd' wiithout path
				else if(c[i].argc == 1)
				{
					char cwd[128];
					if(strcmp(getcwd(cwd,128), getenv("HOME"))==0)
					{
						printf("Current directory is home directory of the user");
						continue;
					}
					else
					{
						chdir(getenv("HOME"));
						continue;
					}
				}
			}

            else if(strcmp(c[i].argv[0],"cwd")==0)
			{
				chdir(c[i].argv[1]);
				continue;
			}
        	//4.2) create child processes (and pipes, redirections etc) 
            
            //Note on claiming zombie processes
            int more = 1; //more zombies to claim
            int status; //trmination status of zombie

            while(more) {
                pid = waitpid(-1, &status, WNOHANG);

                if(pid <= 0) {
                    more = 0;
                }
            }

            //standard input redirection
            if(c->redirect_in != NULL) {
                int fd;
                fd = open(c[i].redirect_in, O_RDONLY | O_CREAT ,  0777);

                if((pid = fork()) < 0)  {
                    perror("Forking error\n");
                    exit(1);
                }

                if(pid == 0) {
                    dup2(fd, STDIN_FILENO);
                    int err = execvp(c[i].argv[0], c[i].argv);

                    if(err == -1) {
                        perror("Could not execute program\n");
                        exit(1);
                    }
                }
            }

            //standard output redirection 
            else if(c->redirect_out != NULL) {

                if((pid = fork()) < 0)  {
                    perror("Forking error\n");
                    exit(1);
                }

                if(pid == 0) {
                    int fd;
                    fd = open(c[i].redirect_out, O_WRONLY | O_CREAT, 0777); 
                    if(fd == -1) { //check error with opening file
                        return 2;
                    }
                    dup2(fd, STDOUT_FILENO);
                    close(fd);
                    
                    if(execvp(c[i].argv[0], c[i].argv) == -1) {
                        perror("Could not execute program\n");
                        exit(1);
                    }
                }
            }

            // Check command line for piping '|'
            else if(strcmp(&(c->com_suffix), "|") == 0) {
                int pipes = 0;

                while (strcmp(&(c->com_suffix), "|") ==0) {
                    i++;
                    pipes++;
                }
                i -= pipes;

                int pipefd[2];
                pid_t cpid;

                if(pipe(pipefd) < 0) {
                    perror("Pipe error\n");
                    exit(1);
                }

                if((pid = fork()) < 0)  {
                    perror("Piping error\n");
                    exit(1);
                }

                if(pid == 0) {
                    dup2(pipefd[1], STDOUT_FILENO);
                    close(pipefd[1]);
                    close(pipefd[0]);

                    if(execvp(c[i].argv[0], c[i].argv) == -1) {
                        perror("Error executing program\n");
                        exit(1);
                    }                    
                }
                else if(pid > 0) {
                    while (strcmp(&(c->com_suffix), "|") ==0) {
                        i++;
                        int file = -1;

                        if(c[i].redirect_out != NULL) {
                            file = open(c[i].redirect_out, O_WRONLY | O_CREAT, 0777);
                        }

                        if(cpid = fork() == -1) {
                            perror("Fork error\n");
                            exit(1);
                        }

                        if(cpid == 0) {
                            if(file != -1) {
                                dup2(file, STDOUT_FILENO);
                                close(pipefd[1]);
                                close(pipefd[0]);
                            }
                        }
                        dup2(pipefd[0], STDIN_FILENO);
                        close(pipefd[0]);
                        close(pipefd[1]);

                        if(execvp(c[i].argv[0], c[i].argv) == -1) {
                            perror("Error executing program\n");
                            exit(1);
                        }
                    }
                }

                if(pid > 0) {
                    close(pipefd[0]);
                    close(pipefd[1]);

                    while(pipes > 0) {
                        wait(&status);
                        --pipes;
                    }
                }
            }

            //standard processing, no piping, no redirection
            else {
                if((pid = fork()) < 0) {
                    perror("Forking error\n");
                    exit(1);
                }

                if(pid == 0) {
                        execvp((c[i].argv[0]),((c[i].argv)));
                        perror("Could not execute program\n");
                        exit(1);
                }
            }

        	//4.3) if the job is a background job (ie ended with &) continue
			//go back for loop
			if(strcmp(&(c[i].com_suffix),"&")==0)
			{
				continue;
			}
		    //4.4)Sequential process, shell will wait for the job to finish
			//In this case until child has finished
			else
			{
				wait(&status);
			}        
        }
    }    

    return 0;
}
