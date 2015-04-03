// Shell Project
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "shell.h"

void shell_init() {
	// init all variables.
	builtin = 0;
        
	// define (allocate storage) for some var/tables

	// init all tables (e.g., alias table)

	// get environment variables (use getenv())
	PATH = getenv("PATH");
	HOME = getenv("HOME");
	PWD = getenv("PWD");
    
	// disable anything that can kill your shell. 
	// (the shell should never die; only can be exit)
	// do anything you feel should be done as init
}

void printPrompt() {
	char pwd[MAX_LENGTH];
	getcwd(pwd, MAX_LENGTH);
	printf(KMAG "[%s] " RESET, getenv("PWD"));
}

int recover_from_errors() {
	// Find out if error occurs in middle of command
	// That is, the command still has a “tail”
	// In this case you have to recover by “eating”
	// the rest of the command.
	// To do this: use yylex() directly. 
}

int getCommand() {
	// http://www.epaperpress.com/lexandyacc/index.html
	//init_scanner_and_parser();
	if (yyparse()) recover_from_errors(); //understand_errors();
	else return OK;
}

void processCommand() {
	// run built-in commands – no fork
	// no exec; only your code + Unix
	// system calls. 
	if (builtin > 0) do_it();
	// execute general commands
	// using fork and exec
	else execute_it();
}

int do_it() {
	/*switch (builtin) {
		case SETENV:
			;
		case PRINTENV:
			;
		case UNSETENV:
			;
		case CD: 
			changedir(newCDPath);
		case CDHOME:
			gohome();
		case ALIAS: // e.g., alias(); alias(name, word);
			;
		case UNALIAS:
			;
		case BYE:
			exit(0);
	}*/
}

int execute_it() {
	// Handle  command execution, pipelining, i/o redirection, and background processing. 
	// Utilize a command table whose components are plugged in during parsing by yacc. 

	// Check Command Accessability and Executability
	/*if (!Executable()) {
		//use access() system call with X_OK
		printf("Command not Found");
		return;
	}
	
	/* Check io file existence in case of io-redirection.
	if (check_in_file() == SYSERR) {
		printf("Cann't read from : %s", srcf);
		return;
	}

	if (check_out_file() == SYSERR) {
		printf("Cann't write to : %s", distf);
		return;
	}*/
	
	//Build up the pipeline (create and set up pipe end points (using pipe, dup) 
	//Process background*/
}

int main(int argc, char *argv[]) {
	shell_init();
	printf(KGRN "Launching [shellX] \n" RESET);

	while(1) {
		printPrompt();
		switch(CMD = getCommand()) {
			case BYE:
				printf(KGRN "Exiting [shellX] \n" RESET);	
				exit(0);
			case ERRORS:	recover_from_errors();
			case OK:	processCommand();
		}
	}
}

