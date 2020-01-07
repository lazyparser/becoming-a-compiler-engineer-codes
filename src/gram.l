%{
#define YYSTYPE char *
#include "gram.tab.h"
%}

INT         [0-9]+
ID     [_a-zA-Z][_a-zA-Z0-9]*

%%

{INT}       { yylval = strdup(yytext); return INT; }
{ID}    { yylval = strdup(yytext); return ID; }

"+"  { return PLUS; }
":=" { return ASSIGN; }

[ \t\n]+          /* eat up whitespace */

%%

int yywrap(void) { 
    return 1;
}