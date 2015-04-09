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
%token <strval> VAR
%token <strval> STRINGLITERAL
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
	| VAR {
		$$ = $1;
	}
	| '~' VAR {
		$$ = $2;
		// look for users homepath here
	}
	| '~' {
		$$ = getenv("HOME");
	}
	| STRINGLITERAL {
		// need to parse inside for alias and env
		printf("Removing quotes \n");
		$$ = $1;
	}
	| '&' {
		printf("external run plz \n");
		$$ = "&";
	}
	| '<' VAR {
		printf("file in \n");
		$$ = $2;
	}
	| '>' VAR {
		printf("file out \n");
		$$ = $2;
	}
	| '2' '>' VAR {
		printf("error out \n");
		$$ = $3;
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
		
		// check first word for alias
		char* alias = xgetalias($1);
		if (alias != NULL) {
			llPush(list, alias, NULL);
			// kick alias back out for reparsing
		}
		else llPush(list, $1, NULL);

		$$ = list;
	}
	| arguments argument {
		llPush($1, $2, NULL);
		$$ = $1;
	};
	
argument:
	variable {
		$$ = $1;
	};
%%

