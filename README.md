# Shell
![GitHub top language](https://img.shields.io/github/languages/top/undisputedcoder/Shell?style=flat-square)
![GitHub code size in bytes](https://img.shields.io/github/languages/code-size/undisputedcoder/Shell?color=blueviolet&style=flat-square)

A command line shell developed by myself and [Juliver](https://github.com/JuliverYbarsabal) a part of an group assignment. 

## Usage
*Note: The program will only run on a Linux operating system and not Windows.*

Download the files and use the command `make` to compile all the project files in the terminal. 

To run the program, type `./main` in the terminal.

Ensure you compile the program before running the program.

Type the command `exit` to quit the program.

## Features & Limitations
The shell is not a fully flegded program and have the listed features and limitations.

### Features
The features of the shell that are functional includes:
- Execute basic commands (i.e. `ls`, `ps`)
- Directory walk
- Standard input and output redirections
- Background job execution (`sleep 10 & ls -l`)
- Sequential job execution (`sleep 10 ; ls -l`)
- Shell pipeline `|` (for short commands)
- Ignore `CTRL-C, CTRL-\, CTRL-Z`
- Claim all zombie processes

### Limitations
The following are limitations of the shell:
1. Pipe a long list of commands
2. Combine different commands 

One main feature that was not implemented was wilcard characters. (i.e `ls -l /tmp/*.c`)

## Testing
The program was tested to the bare minimum and can be improved upon. 

The test cases for testing the shell can be found in the tests.tar.gz file. For this project, the Test-Cases.txt file is applicable and the others can be ignored.

## License
The program is under the MIT license. Feel free to make any contributions or improvements.
