%{
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>

#define MAX_LENGTH	1024

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"
#define RESET "\033[0m"

extern char** environ;

int yyerror(const char *str) {
	fprintf(stderr, "Error: %s\n", str);
	return 0;
}

int yywrap() {
	return 1;
}
%}
%union {
	int intvar;
	char *strval;
}
%token SETENV PRINTENV UNSETENV CD lexALIAS lexUNALIAS LS PWD DEBUG BYE NUMBER
%token <strval> VAR
%token <intval> VAR
%%

commands: 
	/* blank */
	| commands command;

command:
	setenv_case|printenv_case|unsetenv_case|cd_case|alias_case|unalias_case|ls_case|pwd_case|debug_case|bye_case;

setenv_case:
	SETENV VAR VAR {
		printf("set env %s to %s \n", $2, $3);
		setenv($2, $3, 1);
	};

printenv_case:
	PRINTENV {
		int i = 0;
		while (environ[i]) {
			printf("%s \n", environ[i++]);
		}
	};

unsetenv_case:
	UNSETENV VAR {
		printf("unset env %s \n", $2);
		unsetenv($2);
	};

cd_case:
	CD {
		char* home = getenv("HOME");
		chdir(home);
	}
	| CD VAR {
		if (chdir($2) == -1) {
			printf("Error: [%s] is not a valid directory. \n", $2);
		}
	};

alias_case:
	lexALIAS {printf("alias \n");
	};

unalias_case:
	lexUNALIAS {printf("unalias \n");
	};

ls_case: 
	LS {
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
	};

pwd_case:
	PWD {printf("pwd \n");};

debug_case: 
	DEBUG {
		printf("[Debugging info]\n");
		printf("PATH: %s \n", getenv("PATH"));
		printf("HOME: %s \n", getenv("HOME"));
		char pwd[MAX_LENGTH];
		getcwd(pwd, MAX_LENGTH);
		printf("PWD: %s \n", pwd);
	};

bye_case:
	BYE {
		printf(KGRN "Exiting [shellX] \n" RESET);
		exit(0);
	};

/*variable_case:
	VAR {	
		printf("Command not recognized.\n");
		//const char *alias = $1;
//		char *value = value_from_list(linklist, alias);
//		printf("%s\n", value);
	};*/
%%

