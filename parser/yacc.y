%{
#include "shell.h"

void yyerror(const char* str) {
	fprintf(stderr, KRED "Error, %s \n" RESET, str);
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
%token xSETENV xPRINTENV xUNSETENV xCD xALIAS xUNALIAS xDEBUG xBYE xEXIT NEWLINE
%token <strval> VAR
%type <strval> variable
%type <linkedlist> ARGS
%type <strval> ARG
%%

commands: 
	/* blank */
	| commands command {
		// print current directory
		char* pwd = get_current_dir_name();
		printf(KBLU "%s> " RESET, pwd);
	};

command:
	variable
	| setenv end
	| printenv end
	| unsetenv end
	| cd end
	| alias end
	| unalias end
	| debug end
	| bye end
	| exit end
	| cmd end;

end:
	NEWLINE {
		YYACCEPT;
	};

// builtins
variable:
	'$' '{' VAR '}' {
		char* value = getenv($3);
		if (value == NULL) yyerror("variable is not defined");
		else printf("Replacing %s with %s \n", $3, value);
		$$ = value;
	}
	| '"' VAR '"' {
		printf("Removing quotes\n");
		$$ = $2;
	}
	| VAR {
		$$ = $1;
	};

setenv:
	xSETENV variable variable {
		xsetenv($2, $3);
	};

printenv:
	xPRINTENV {
		xprintenv();
	};

unsetenv:
	xUNSETENV variable {
		xunsetenv($2);
	};

cd:
	xCD {
		xcd(NULL);
	}
	| xCD variable {
		xcd($2);
	};

alias:
	xALIAS {
		xprintalias();
	}
	| xALIAS variable {
		xgetalias($2);
	}
	| xALIAS variable variable {
		xsetalias($2, $3);
	};

unalias:
	xUNALIAS VAR {
		xunalias($2);
	};

debug: 
	xDEBUG {
		xdebug();
	};

bye:
	xBYE {
		xbye();
	};

exit:
	xEXIT {
		printf("Did you mean " KGRN "bye" RESET "? \n");
	};

// No match, could be exc command with argms	
cmd:
	ARGS {
		xexecute($1);
	};

ARGS:
	ARG {
		// First word, check for alias here?
		ll* list = llCreate(1);
		llPush(list, $1, NULL);
		$$ = list;
	}
	| ARGS ARG {
		llPush($1, $2, NULL);
		$$ = $1;
	};
	
ARG:
	variable {
		$$ = $1;
	}
	| '&' {
		printf("and\n");
		$$ = (char*) '&';
	};
%%

