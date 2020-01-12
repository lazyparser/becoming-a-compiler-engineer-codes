%{
    #define YYSTYPE char *
    #include "parsing.tab.h"
%}

NUMBER     [0-9]+
ID         [_a-zA-Z][_a-zA-Z0-9]*

%%
{NUMBER}     {yylval = strdup(yytext); return NUMBER;}
{ID}         {yylval = strdup(yytext); return ID;}
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
