// Shell Project
#include "shell.h"

//
// Built-ins
//
void xsetenv() {
	int success = setenv(setenvName, setenvValue, 1);
	if (success != 0) printf("Error: Could not set %s as %s. \n", setenvName, setenvValue);
}

void xprintenv() {
	int i = 0;
	while (environ[i]) {
		printf("%s \n", environ[i++]);
	}
}

void xunsetenv() {
	int success = unsetenv(unsetenvName);
	if (success != 0) printf("Error: Could not remove %s. \n", unsetenvName);
}

void xcd() {
	if (cdPath == NULL) {
		char* home = getenv("HOME");
		chdir(home);
	}
	else {
		if (chdir(cdPath) == -1) {
			printf("Error: %s is not a valid directory. \n", cdPath);
		}
	}
}

void xalias() {
	if (aliasName == NULL && aliasValue == NULL) {
		llPrint(aliasTable);
	}
	else {
		printf("alias %s to %s \n", aliasName, aliasValue);
		llPush(aliasTable, aliasName, aliasValue);
	}
}

void xunalias() {
	printf("unalias %s \n", unaliasName);
	llRemove(aliasTable, unaliasName);
}

void xls() {
	DIR *directory;
	directory = opendir(".");

	struct dirent *fileListing;

	if (directory) {
		while ((fileListing = readdir(directory)) != NULL) {
			printf("%s \n", fileListing->d_name);
		}
		closedir(directory);
	}
	else {
		printf("Error: Could not get list of files. \n");
	}
}

void xpwd() {
	char* pwd = get_current_dir_name();
	printf("%s \n", pwd);
}

void xdebug() {
	printf("[Debugging info]\n");
	printf("PATH: %s \n", getenv("PATH"));
	printf("HOME: %s \n", getenv("HOME"));
	char pwd[MAX_LENGTH];
	getcwd(pwd, MAX_LENGTH);
	printf("PWD: %s \n", pwd);
}

void xbye() {
	printf(KGRN "Exiting [shellX] \n" RESET);
	llFree(aliasTable);
	exit(0);
}

//
// shellX code
//
void shell_init() {
	// init alias table
	aliasTable = llCreate();

	/*llPush(aliasTable, "test", "testvalue");
	llPush(aliasTable, "test2", "test2value");
	llPush(aliasTable, "test3", "test3value");
	llPush(aliasTable, "test4", "test4value");
	llPush(aliasTable, "test5", "test5value");

	llPrint(aliasTable);

	printf("fetch test3 \n");
	printf("%s \n", llGet(aliasTable, "test3"));

	llPrint(aliasTable);

	printf("remove from start \n");
	llRemove(aliasTable, "test");
	llPrint(aliasTable);

	printf("remove from end \n");
	llRemove(aliasTable, "test5");
	llPrint(aliasTable);
	
	printf("remove from middle \n");
	llRemove(aliasTable, "test3");
	llPrint(aliasTable);

	printf("fetch test4 \n");
	printf("%s \n", llGet(aliasTable, "test4"));

	printf("remove from start \n");
	llRemove(aliasTable, "test2");
	llPrint(aliasTable);

	printf("remove only child \n");
	llRemove(aliasTable, "test4");

    printf("print the empty table \n");
	llPrint(aliasTable);

	llPush(aliasTable, "test", "testvalue");
	llPush(aliasTable, "test2", "test2value");
	llPush(aliasTable, "test3", "test3value");
	llPush(aliasTable, "test4", "test4value");
	llPush(aliasTable, "test5", "test5value");

	printf("print table \n");
	llPrint(aliasTable);*/

	// get environment variables (use getenv())
	PATH = getenv("PATH");
	HOME = getenv("HOME");
    
	// disable anything that can kill your shell. 
	signal(SIGINT, SIG_IGN);  // Ctrl-C
    signal(SIGQUIT, SIG_IGN); // Ctrl-backslash
    signal(SIGTSTP, SIG_IGN); // Ctrl-Z
    // cannot disable 'kill'
}

int recover_from_errors() {
	// Find out if error occurs in middle of command
	// That is, the command still has a “tail”
	// In this case you have to recover by “eating”
	// the rest of the command.
	// To do this: use yylex() directly.
	printf("An exception has occured. \nRecovering... ");
	
	while (yylex() != 0) {
	    printf("... ");
	}
	
	printf("\n");
}

int getCommand() {
	//init_scanner_and_parser();
	if (yyparse()) {
		//understand_errors();
		CMD = SYSERR;
		return;
	}
	else return OK;
}

int do_it() {
	switch (builtin) {
		case SETENV:
			xsetenv();
			break;
		case PRINTENV:
			xprintenv();
			break;
		case UNSETENV:
			xunsetenv();
			break;
		case CD: 
			xcd();
			break;
		case ALIAS:
			xalias();
			break;
		case UNALIAS:
			xunalias();
			break;
		case LS:
			xls();
			break;
		case PWD:
			xpwd();
			break;
		case DEBUG:
			xdebug();
			break;
	}
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

	while (1) {
		// print current directory
		char* pwd = get_current_dir_name();
		printf(KMAG "%s> " RESET, pwd);

		// process yacc
		getCommand();

		switch (CMD) {
			case BYE:
				xbye();
				break;
			case SYSERR:
				recover_from_errors();
				break;
			case OK:
				if (builtin != 0) do_it();
				else execute_it();
				break;
		}
	}
}

