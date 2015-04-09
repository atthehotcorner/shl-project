%{
#include "shell.h"

void yyerror(const char* str) {
	fprintf(stderr, KRED "Error, %s \n" RESET, str);
	// unusable command chain
	llFree(chain);
	chain = NULL;
}

int yywrap() {
	return 1;
}

extern int yylineno;
%}
%error-verbose
%union {
	int intvar;
	char* strval;
	void* linkedlist;
}
%token NEWLINE
%token <strval> VAR
%type <strval> variable
%type <linkedlist> arguments
%type <strval> argument
%%

commands: 
	/* blank */
	| commands command;

command:
	xcommand;

variable:
	'$' '{' VAR '}' {
		char* value = getenv($3);
		if (value == NULL) {
			yyerror("variable is not defined");
			YYERROR;
		}
		printf("Replacing %s with %s \n", $3, value);
		$$ = value;
	}
	| '"' VAR '"' {
		printf("Removing quotes\n");
		$$ = $2;
	}
	| VAR {
		$$ = $1;
	}
	| '~' VAR {
		$$ = $2;
		// look for users homepath here
	}
	| '~' {
		$$ = getenv("HOME");
	};

xcommand:
	arguments {
		chain = $1;
		YYACCEPT;
	};

arguments:
	argument {
		// First word
		ll* list = llCreate(1);
		llPush(list, $1, NULL);
		$$ = list;
	}
	| arguments argument {
		llPush($1, $2, NULL);
		$$ = $1;
	};
	
argument:
	variable {
		$$ = $1;
	}
%%

