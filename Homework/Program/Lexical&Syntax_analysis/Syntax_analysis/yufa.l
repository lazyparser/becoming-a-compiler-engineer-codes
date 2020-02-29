%{
#include "yufa.tab.h"
%}
NUM	[0-9]+
OP 	[-+*/]
%%
{NUM}          { printf("A NUM: %s\n",yytext);yylval = atoi(yytext); return NUM; }
{OP}      { printf("An Operator: %s\n",yytext);return yytext[0]; }	
[(]      { printf("A LP: %s\n",yytext);return yytext[0]; }	
[)]      { printf("A RP: %s\n",yytext);return yytext[0]; }	
[\n]      { return yytext[0]; }	
.               { return 0;  }
%%

int yywrap(void) { 
    return 1;
}