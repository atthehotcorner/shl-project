//
// Shell Project
//
#include "shell.h"

//
// Built-ins
//
void xsetenv(char* name, char* value) {
	int success = setenv(name, value, 1);
	if (success == 0) printf("Variable %s set to %s. \n", name, value);
	else printf(KRED "Error, could not set %s as %s. \n" RESET, name, value);
}

void xprintenv() {
	int i = 0;
	while (environ[i]) {
		printf("%s \n", environ[i++]);
	}
}

void xunsetenv(char* name) {
	int success = unsetenv(name);
	if (success != 0) printf(KRED "Error, could not remove %s. \n" RESET, name);
}

void xcd(char* path) {
	if (path == NULL) {
		char* home = getenv("HOME");
		chdir(home);
	}
	else {
		if (chdir(path) == -1) {
			printf(KRED "Error, %s is not a valid directory. \n" RESET, path);
		}
	}
}
	
char* xgetalias(char* name) {
	char* temp = llGetAlias(aliasTable, name);
	//if (temp == NULL) printf(KRED "Error, %s was not found or is circular. \n" RESET, name);
	return temp;
}

void xsetalias(char* name, char* value) {
	printf("Alias %s set to %s. \n", name, value);
	llPushAlias(aliasTable, name, value);
}

void xprintalias() {
	llPrint(aliasTable);
}

void xunalias(char* name) {
	printf("Alias %s removed. \n", name);
	llRemove(aliasTable, name);
}

void xdebug() {
	printf("[Debugging info]\n");
	printf("PATH: %s \n", getenv("PATH"));
	printf("HOME: %s \n", getenv("HOME"));
	printf("PWD: %s \n", get_current_dir_name());
}

void xbye() {
	printf(KGRN "Exiting [shellX] \n" RESET);
	llFree(aliasTable);
	exit(0);
}

//
// Externals
//
void xexecute(ll* list) {
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

	//printf("Executing %s \n", list->start->name);

	// check for alias here? (does not parse flags in alias)
	char* alias = xgetalias(list->start->name);
	if (alias != NULL) {
		list->start->name = alias;
	}

	// See if command exists
	char* path = xpathlookup(list->start->name);

	if (path != NULL) {
		// matched command from path
		list->start->name = path;
	}
	else {
		// invalid command, exit
		printf(KRED "Error, could not find command %s. \n" RESET, list->start->name);
		return;
	}

	// Time to execute
	int status;
	pid_t pid = fork();

	if (pid == 0) {
		xexecutecommand(list);
		exit(0); // if command fails
	}
	else {
		llFree(list); // no longer needed
		waitpid(pid, &status, 0);
	}
}

char* xpathlookup(char* command) {
	// command already absolute, works as given
	if (access(command, X_OK) == 0) return command;

	// look if command is in one of the paths
	char* path = strdup(getenv("PATH"));
	char* parsedPath = strtok(path, ":");

	while (parsedPath != NULL) {
		char* result = malloc(sizeof(command) + sizeof(parsedPath) + 2); // +1 for null, +1 for slash
		strcpy(result, parsedPath);
    	strcat(result, "/");
    	strcat(result, command);

		// found the winning path!
		if (access(result, X_OK) == 0) {
			return result;
		}

		// next token
		//printf("%s \n", result);
		free(result); // bug when command is over 8 chars
		result = NULL;
		parsedPath = strtok(NULL, ":");
	}

	// not found
	return NULL;
}

void xexecutecommand(ll* list) {
	// http://geoffgarside.co.uk/2009/08/28/using-execve-for-the-first-time/
	node* current = list->start;

	// Parse list into argv
	char** envp = {NULL};
	char** argv = malloc(sizeof(char*) * (list->count + 1));
	argv[list->count] = NULL; // NULL terminator
	
	int i;
	// no while loop, we need an index for argv
	for (i = 0; i < list->count; i++) {
		argv[i] = current->name;
		current = current->next;
	}

	execve(argv[0], argv, envp); // or execv
}

//
// shellX code
//
void shell_init() {
	// init alias table
	aliasTable = llCreate(0);

    llPush(aliasTable, "a", "c");
	llPush(aliasTable, "b", "d");
	llPush(aliasTable, "c", "a");
	llPush(aliasTable, "d", "dfinal");
	llPush(aliasTable, "q", "pwd");
	llPush(aliasTable, "w", "ls");

	/* get environment variables --NOT USED--
	PATH = getenv("PATH");
	HOME = getenv("HOME");*/
    
	// disable anything that can kill your shell. 
	/*signal(SIGINT, SIG_IGN);  // Ctrl-C
    signal(SIGQUIT, SIG_IGN); // Ctrl-backslash
    signal(SIGTSTP, SIG_IGN); // Ctrl-Z*/
}

int recover_from_errors() {
	/*printf(KRED "An exception has occured. \nRecovering [%s]... ", yytext);

	int x = yylex();
	while (x != 0 && x != 59) {
	    printf("[%s] %d ... ", yytext, x);
	}
	printf("\n" RESET);*/
	
	printf(KRED "An exception has occured. \n");
}

int main(int argc, char *argv[]) {
	shell_init();
	printf(KGRN "Launching [shellX] \n" RESET);

	while (1) {
		// print current directory
		char* pwd = get_current_dir_name();
		printf(KMAG "%s> " RESET, pwd);
	
		// start processing
		//yyparse();
		if (yyparse() == 1) recover_from_errors();
	}
}

