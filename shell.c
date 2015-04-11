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
	else printf(KRED "[xshell] could not set %s as %s. \n" RESET, name, value);
}

void xprintenv() {
	int i = 0;
	while (environ[i]) {
		printf("%s \n", environ[i++]);
	}
}

void xunsetenv(char* name) {
	int success = unsetenv(name);
	if (success != 0) printf(KRED "[xshell] could not remove %s. \n" RESET, name);
}

void xcd(char* path) {
	if (path == NULL) {
		char* home = getenv("HOME");
		chdir(home);
	}
	else {
		if (chdir(path) == -1) {
			printf(KRED "[xshell] %s is not a valid directory. \n" RESET, path);
		}
	}
}
	
char* xgetalias(char* name) {
	char* temp = llGetAlias(aliasTable, name);
	//if (temp == NULL) printf(KRED "[xshell] %s was not found or is circular. \n" RESET, name);
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

void xbye() {
	printf(KCYN "leaving [xshell] \n" RESET);
	llFree(aliasTable);
	chainReset(chainTable);
	free(chainTable);
	exit(0);
}

//
// Externals
//
void xexecute(ll* list) {
	//fprintf(stderr, KGRN "Executing %s \n" RESET, list->start->name);

	// See if command exists
	char* path = xpathlookup(list->start->name);

	if (path != NULL) {
		// matched command from path
		list->start->name = path;
	}
	else {
		// invalid command, exit
		fprintf(stderr, KRED "[xshell] could not find command %s. \n" RESET, list->start->name);
		return;
	}

	// Time to execute
	int status;
	int background = chainTable->background; // 0 - not in background, 1 - in bg
	pid_t pid = fork();

	if (pid == 0) {
		node* current = list->start;

		// Parse list into argv
		char** argv = calloc(list->count + 1, sizeof(char*));
		argv[list->count] = NULL; // NULL terminator

		int i;
		// no while loop, we need an index for argv
		for (i = 0; i < list->count; i++) {
			argv[i] = current->name;
			current = current->next;
		}

		execv(argv[0], argv);

		exit(0); // if returned here fatal error
	}
	else {
		llFree(list); // no longer needed
		if (background != 1) waitpid(pid, &status, 0);
	}
}

char* xpathlookup(char* command) {
	// command already absolute, works as given
	if (access(command, X_OK) == 0) return command;

	// look if command is in one of the paths
	char* path = strdup(getenv("PATH"));
	char* parsedPath = strtok(path, ":");

	while (parsedPath != NULL) {
		char* result = malloc(strlen(command) + strlen(parsedPath) + 2); // +1 for null, +1 for slash
		strcpy(result, parsedPath);
    	strcat(result, "/");
    	strcat(result, command);

		// found the winning path!
		if (access(result, X_OK) == 0) {
			return result;
		}

		// next token
		free(result);
		result = NULL;
		parsedPath = strtok(NULL, ":");
	}

	// not found
	return NULL;
}

//
// xshell code
//
void min() {
	fprintf(stderr, KRED "Missing required arguments. \n" RESET);
	return;
}

void ignore(int number) {
	if (number < 2) fprintf(stderr, KYEL "Ignoring last additional argument. \n" RESET);
	else fprintf(stderr, KYEL "Ignoring last %d additional arguments. \n" RESET, number);
}

void restoreio() {
   	fflush(stdin);
   	dup2(defaultstdin, STDIN_FILENO);
  	fflush(stdout);
   	dup2(defaultstdout, STDOUT_FILENO);
   	fflush(stderr);
	dup2(defaultstderr, STDERR_FILENO);
}

void xshell(ll* list) {
	if (list == NULL || list->start == NULL) return;
	char* command = list->start->name;
	int count = list->count;

	// check for io redirections	
	if (chainTable->fileIn != NULL) {
		if (freopen(chainTable->fileIn, "r", stdin) == NULL) {
			// couldnt open file
			restoreio();
			fprintf(stderr, "Problem opening %s for input. \n", chainTable->fileIn);
			chainReset(chainTable); // kill following cmds
		}
		fprintf(stderr, "Input from %s \n", chainTable->fileIn);
	}

	if (chainTable->fileOut != NULL) {
		char* mode = (chainTable->fileOutMode == 1)? "a":"w";

		if (freopen(chainTable->fileOut, mode, stdout) == NULL) {
			// couldnt open file
			restoreio();
			fprintf(stderr, "Problem opening %s for output. \n", chainTable->fileOut);
			chainReset(chainTable); // kill following cmds
		}
		fprintf(stderr, "Output to %s \n", chainTable->fileOut);
	}
	
	if (chainTable->fileErrorOut != NULL) {
		if (freopen(chainTable->fileErrorOut, "a", stderr) == NULL) {
			// couldnt open file
			restoreio();
			fprintf(stderr, "Cannot open %s as File IO_ERR \n", chainTable->fileErrorOut);
			chainReset(chainTable); // kill following cmds
		}
		fprintf(stderr, "STDError to %s \n", chainTable->fileErrorOut);
	}
	else if (chainTable->fileErrorStdout == 1) {
		fprintf(stderr, "STDError to stdout \n");
		dup2(STDOUT_FILENO, STDERR_FILENO);
	}

	if (chainTable->background == 1) {
		fprintf(stderr, "Process in background \n");
	}

	// check if built in or command
	if (strcmp(command, "setenv") == 0) {
		if (count < 3) {min(); return;}
		else if (count > 3) ignore(count - 3);

		llPop(list); // ignore command name
		char *a = llPop(list), *b = llPop(list);
		xsetenv(a, b);
	}

	else if (strcmp(command, "printenv") == 0) {
		if (count > 1) ignore(count - 1);
		xprintenv();	
	}

	else if (strcmp(command, "unsetenv") == 0) {
		if (count < 2) {min(); return;}
		else if (count > 2) ignore(count - 2);

		llPop(list); // ignore command name
		xunalias(llPop(list));
	}

	else if (strcmp(command, "cd") == 0) {
		if (count > 2) ignore(count - 2);
		
		llPop(list); // ignore command name
		xcd(llPop(list));
	}

	else if (strcmp(command, "alias") == 0) {
		if (count > 3) ignore(count - 3);

		llPop(list); // ignore command name
		char* name = llPop(list);
		char* value = llPop(list);

		if (name != NULL) {
			if (value != NULL) {
				xsetalias(name, value);
			}
			else {
				xgetalias(name);
			}
		}
		else {
			xprintalias();
		}
	}

	else if (strcmp(command, "unalias") == 0) {
		if (count > 1) ignore(count - 1);
		
		llPop(list); // ignore command name
		xunalias(llPop(list));
	}

	else if (strcmp(command, "bye") == 0) {
		xbye();
	}

	else {
		// try executing
		xexecute(list);
	}

	// return io redirections to normal
	restoreio();
}

char* getaline() {
	// http://stackoverflow.com/a/314422
    char * line = malloc(100), * linep = line;
    size_t lenmax = 100, len = lenmax;
    int c;

    if(line == NULL)
        return NULL;

    for(;;) {
        c = fgetc(stdin);
        if(c == EOF)
            break;

        if(--len == 0) {
            len = lenmax;
            char * linen = realloc(linep, lenmax *= 2);

            if(linen == NULL) {
                free(linep);
                return NULL;
            }
            line = linen + (line - linep);
            linep = linen;
        }

        if((*line++ = c) == '\n')
            break;
    }
    *line = '\0';
    return linep;
}

char* expand_environment_variables(char* s) {
	// http://stackoverflow.com/a/20715800
	char* x = strstr(s, "${");
	char* xx = strstr(s, "}");
    if (x == NULL || xx == NULL) return s;

	int length = strlen(s);
	int preSize = x-s;
	int postSize = length - (xx-s) - 1; // -1 for }
	int varSize = length - preSize - postSize - 3; // -3 for ${}

	char* pre = malloc(sizeof(char) * (preSize + 1));
	char* post = malloc(sizeof(char) * (postSize + 1));
	char* var = malloc(sizeof(char) * (varSize + 1));

	int i;
	for (i = 0; s[i]; i++) {
	    if (i < preSize) pre[i] = s[i];
	    if (i > preSize+1 && i < preSize+varSize+2) var[i-(preSize+2)] = s[i];
	    if (i > preSize+varSize+2) post[i-(preSize+varSize+3)] = s[i];
	}
	
	char* y = getenv(var);
	if (y == NULL) {
		fprintf(stderr, KRED "[xshell] %s is not a valid environment variable. \n" RESET, var);
		return NULL;
	}
    
    strcat(pre, y);
    strcat(pre, post);
    //if (post != NULL) free(post);
    //if (var != NULL) free(var);
    return expand_environment_variables(pre);
}

void shell_init() {
	// init storage
	aliasTable = llCreate(0);
	chainTable = chainCreate(0);
	chainBuffer = NULL;
	defaultstdin = dup(STDIN_FILENO);
    defaultstdout = dup(STDOUT_FILENO);
    defaultstderr = dup(STDERR_FILENO);

    /*llPush(aliasTable, "a", "c");
	llPush(aliasTable, "b", "d");
	llPush(aliasTable, "c", "a");
	llPush(aliasTable, "d", "dfinal");
	llPush(aliasTable, "q", "pwd");
	llPush(aliasTable, "w", "ls");
	
	ll* a = llCreate(1);
	llPush(a, "0", NULL);
	llPush(a, "1", NULL);
	llPush(a, "2", NULL);
	
	ll* b = llCreate(1);
	llPush(a, "b", NULL);
	llPush(a, "c", NULL);
	llPush(a, "d", NULL);*/
    
	// disable anything that can kill your shell. 
	/*signal(SIGINT, SIG_IGN);  // Ctrl-C
    signal(SIGQUIT, SIG_IGN); // Ctrl-backslash
    signal(SIGTSTP, SIG_IGN); // Ctrl-Z*/
}

int recover_from_errors() {
	fprintf(stderr, KRED "[xshell] Cannot continue, recovering... \n" RESET);
}

int main(int argc, char *argv[]) {
	shell_init();
	printf(KCYN "starting [xshell] \nbuilt %s %s \n" RESET, __DATE__, __TIME__);

	while (1) {
		// print current directory
		char* pwd = get_current_dir_name();
		printf(KMAG "%s> " RESET, pwd);

		// get users input
		char* input = getaline();

		// process input
		char* input2 = expand_environment_variables(input);
		
		//printf(KCYN "%s" RESET, input2);
		
		yy_scan_string(input);
		if (yyparse() == 1) recover_from_errors();

		// process chain
		//chainPrint(chainTable);
		
		ll* command = chainPop(chainTable);
		while (command != NULL) {
			xshell(command);
			command = chainPop(chainTable);
		}

		// free up memory
		free(input);
		chainReset(chainTable);
		
		//fprintf(stderr, "clear the table \n");
	}
}

