%{
    #define YYSTYPE char *
    #include "yufa.tab.h"
%}

NUMBER     [0-9]+

%%
{NUMBER}     {yylval = strdup(yytext); return NUMBER;}

"*"          {return MUL;}
"/"          {return DIV;}
":="         {return EQUAL;}

%%
int yywrap(void)
{
    return 1;
}
