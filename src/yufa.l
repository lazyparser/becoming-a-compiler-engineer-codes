

%{

    #define YYSTYPE char *

    #include "yufa2.tab.h"

%}



DIGIT     [0-9]+
LETTER    [a-zA-Z]+ 


%%

{LETTER}    {yylval = strdup(yytext); return LETTER;}
{DIGIT}     {yylval = strdup(yytext); return DIGIT;}


"+"          {return ADD;}

"-"          {return SUB;}

"*"          {return MUL;}

"/"          {return DIV;}

":="         {return ABS;}



%%

int yywrap(void)

{

    return 1;

}
