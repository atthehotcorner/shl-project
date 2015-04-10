%{
#include <stdio.h>
#include "y.tab.h"
%}	
/* http://dinosaur.compilertools.net/yacc/
[a-zA-Z0-9\/.-]+
*/
VAR [0-9a-zA-Z/:.~\-?*] 
%%

setenv			return xSETENV;
printenv		return xPRINTENV;
unsetenv		return xUNSETENV;
cd 				return xCD;
alias			return xALIAS;
unalias			return xUNALIAS;
debug			return xDEBUG;
bye				return xBYE;
exit			return xEXIT;
quit			return xEXIT;

\"				return (int) '"';
\{ 				return (int) '{';
\} 				return (int) '}';
\; 				return (int) ';';
\&				return (int) '&';

{VAR}+ 			yylval.strval = strdup(yytext); return VAR;

\n				return NEWLINE;
[ \t\r]+		/* whitespace */;
. 				/* ignore */;
%%

