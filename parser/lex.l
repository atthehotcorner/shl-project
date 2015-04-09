%{
#include <stdio.h>
#include "y.tab.h"
%}	
VAR [0-9a-zA-Z/:.~\-?*] 
%%

\"				return '"';
\{ 				return '{';
\} 				return '}';
\; 				return ';';
\$ 				return '$';
\&				return '&';
\~				return '~';

{VAR}+ 			yylval.strval = strdup(yytext); return VAR;

\n				return 0;
[ \t\r]+		/* whitespace */;
. 				yyerror("input is not valid");
%%

