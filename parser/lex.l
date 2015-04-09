%{
#include <stdio.h>
#include "y.tab.h"
%}
/* http://www.lysator.liu.se/c/ANSI-C-grammar-l.html */
STRINGLITERAL [a-zA-Z_]?\"(\\.|[^\\"])*\"
VAR [0-9a-zA-Z/:.~\-?*]
%%

"|"					return '|';
"{"					return '{';
"}"					return '}';
";"					return ';';
"$"					return '$';
"&"					return '&';
"~"					return '~';
"<"					return '<';
">"					return '>';
"2>&1"				return STDERROUT;

{STRINGLITERAL}		{
						yylval.strval = (char*)calloc(strlen(yytext)-1, sizeof(char));
						strncpy(yylval.strval, &yytext[1], strlen(yytext)-2);
						return STRINGLITERAL;
					}

{VAR}+ 				yylval.strval = strdup(yytext); return VAR;

\n					return 0;
[ \t\r]+			/* whitespace */;
. 					yyerror("input is not valid");
%%

