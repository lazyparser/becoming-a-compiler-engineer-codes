%{
#define YYSTYPE char *
#include "parsing.tab.h"
#include<stdlib.h>
%}

digits    [0-9]+
Id        [_A-Za-z][A-Za-z0-9]+

%%

{digits}    {//printf("%s\n",yylval);
             yylval = strdup(yytext); return NUMBER;}
{Id}        {yylval = strdup(yytext); return ID;}
"+"         {return ADD;}
"-"         {return SUB;}
"*"         {return MUL;}
"/"         {return DIV;}
":="        {return ASSIGN;}

%%
int yywrap(void)
{
	return 1;
}
