%{
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "shell.h"

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
%token xSETENV xPRINTENV xUNSETENV xCD xALIAS xUNALIAS xLS xPWD xDEBUG xBYE NUMBER
%token <strval> VAR
%token <intval> VAR
%%

commands: 
	/* blank */
	| commands command;

command:
	setenv_case|printenv_case|unsetenv_case|cd_case|alias_case|unalias_case|ls_case|pwd_case|debug_case|bye_case;

setenv_case:
	xSETENV VAR VAR {
		CMD = OK;
		builtin = SETENV;
		setenvName = $2;
		setenvValue = $3;
	};

printenv_case:
	xPRINTENV {
		CMD = OK;
		builtin = PRINTENV;
	};

unsetenv_case:
	xUNSETENV VAR {
		CMD = OK;
		builtin = UNSETENV;
		unsetenvName = $2;
	};

cd_case:
	xCD {
		CMD = OK;
		builtin = CD;
		cdPath = NULL;
	}
	| xCD VAR {
		CMD = OK;
		builtin = CD;
		cdPath = $2;
	};

alias_case:
	xALIAS {
		CMD = OK;
		builtin = ALIAS;
		aliasName = NULL;
		aliasValue = NULL;
	}
	| xALIAS VAR VAR {
		CMD = OK;
		builtin = ALIAS;
		aliasName = $2;
		aliasValue = $3;
	};

unalias_case:
	xUNALIAS VAR {
		CMD = OK;
		builtin = UNALIAS;
		unaliasName = $2;
	};

ls_case: 
	LS {
		CMD = OK;
		builtin = LS;
	};

pwd_case:
	PWD {
		CMD = OK;
		builtin = PWD;
	};

debug_case: 
	DEBUG {
		CMD = OK;
		builtin = DEBUG;
	};

bye_case:
	BYE {
		CMD = OK;
		builtin = BYE;
	};

/*variable_case:
	VAR {	
		printf("Command not recognized.\n");
		//const char *alias = $1;
//		char *value = value_from_list(linklist, alias);
//		printf("%s\n", value);
	};*/
%%

