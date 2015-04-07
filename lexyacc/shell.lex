%{
#include <stdio.h>
#include "y.tab.h"
%}	
/* http://dinosaur.compilertools.net/yacc/
[a-zA-Z0-9\/.-]+
*/
VAR [0-9a-zA-Z/:.~\-?*] 
%%

setenv				return xSETENV;
printenv			return xPRINTENV;
unsetenv			return xUNSETENV;
cd 					return xCD;
alias				return xALIAS;
unalias				return xUNALIAS;
debug				return xDEBUG;
bye					return xBYE;
{VAR}+ 				{yylval.strval = strdup(yytext); return VAR;}

'<'                 return LT;
'>'                 return GT;
'&'                 return AMP;
'\"'				return DQUOTE;
'$'					return DRSIGN;
'\{'				return OPNBRACE;
'\}					return CLSBRACE;
'\\'                return BACKSLASH;
'|'					return PIPE;
'~'					return TILDE;

';'					return 0;
\n					return END;
<<EOF>>				return 0;
[ \t\r]+			{;} /* whitespace */
.            	  	{;} /* ? */
%%

