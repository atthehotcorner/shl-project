%{
#include <stdio.h>
#include "y.tab.h"
%}	
%%
setenv			return SETENV;
printenv		return PRINTENV;
unsetenv		return UNSETENV;
cd			return CD;
alias			return lexALIAS;
unalias			return lexUNALIAS;
ls			return LS;
pwd			return PWD;
debug			return DEBUG;
bye			return BYE;
[0-9]+			return NUMBER;
[0-9a-zA-Z/:.~\-?*]+	{yylval.strval = strdup(yytext); return VAR;}
\n			return 0; /* enter keypress */
[ \t\r]+		{;} /* whitespace */
.            	  	{;} /* catchall */
%%
