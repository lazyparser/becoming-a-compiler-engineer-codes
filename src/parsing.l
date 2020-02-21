%{
    #define YYSTYPE char *
    #include "parsing.tab.h"
    #include <stdlib.h> 
%}

NUMBER     [0-9]+
Id         [_A-Za-z][A-Za-z0-9]+

%%
{NUMBER}     {yylval = strdup(yytext); return NUMBER;}
{Id}         {yylval = strdup(yytext); return ID;}
"+"          {return ADD;}
"-"          {return SUB;}
"*"          {return MUL;}
"/"          {return DIV;}
":="         {return EQ;}

%%
int yywrap(void)
{
    return 1;
}
