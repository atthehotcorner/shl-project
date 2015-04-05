%{
#include <stdio.h>
#include "y.tab.h"
%}	
%%
setenv					return xSETENV;
printenv				return xPRINTENV;
unsetenv				return xUNSETENV;
cd 						return xCD;
alias					return xALIAS;
unalias					return xUNALIAS;
ls						return xLS;
pwd						return xPWD;
debug					return xDEBUG;
bye						return xBYE;
[0-9a-zA-Z/:.~\-?*]+	{yylval.strval = strdup(yytext); return VAR;}
\n						{return 0;} /* enter keypress */
<<EOF>>					{return 0;}
[ \t\r]+				{;} /* whitespace */
.            	  		{;} /* catchall */
%%
