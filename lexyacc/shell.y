%{
#include "shell.h"

void yyerror(const char* str) {
	fprintf(stderr, KRED "Error, %s \n" RESET, str);
}

int yywrap() {
	return 1;
}
%}
%union {
	int intvar;
	char* strval;
	void* linkedlist;
}
%token xSETENV xPRINTENV xUNSETENV xCD xALIAS xUNALIAS xDEBUG xBYE END
%token LT GT AMP DQUOTE DRSIGN OPNBRACE CLSBRACE BACKSLASH PIPE TILDE
%token <strval> VAR
%type <linkedlist> ARGS
%type <strval> ARG
%%

commands: 
	/* blank */
	| commands command {
		// print current directory
		char* pwd = get_current_dir_name();
		printf(KMAG "%s> " RESET, pwd);
	};

command:
	END {} 
	| getenv END
	| setenv END
	| printenv END
	| unsetenv END
	| cd END
	| alias END
	| unalias END
	| debug END
	| bye END
	| cmd END;

// builtins
getenv:
	DRSIGN OPNBRACE VAR CLSBRACE {
		printf("looking up [%s] \n", $3);
		char* value = getenv($3);
		if (value == NULL) yyerror("variable is not defined");
		else printf("%s \n", value);
	};

setenv:
	xSETENV VAR VAR {
		xsetenv($2, $3);
	};

printenv:
	xPRINTENV {
		xprintenv();
	};

unsetenv:
	xUNSETENV VAR {
		xunsetenv($2);
	};

cd:
	xCD {
		xcd(NULL);
	}
	| xCD VAR {
		xcd($2);
	};

alias:
	xALIAS {
		xprintalias();
	}
	| xALIAS VAR {
		xgetalias($2);
	}
	| xALIAS VAR VAR {
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
	}
	
ARG:
	VAR {
		$$ = $1;
	};

%%

