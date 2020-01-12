%{
#define YYSTYPE char *
#include "parsing.tab.h"
%}

digits    [0-9]+
Id        [_A-Za-z][A-Za-z0-9]+
WHILe     ["WHILE"|"while"]
BEGIn     ["BEGIN"|"begin"]
ENd       ["END"|"end"]     
Do        ["DO"|"do"]       
If        ["IF"|"if"]       
THEn      ["THEN"|"then"]   
ELSe      ["ELSE"|"else"] 

%%

{digits}    {yylval = strdup(yytext); return NUMBER;}
{Id}        {yylval = strdup(yytext); return ID;}
{WHILe}     {return WHILE;}
{BEGIn}     {return BEGIn;}
{ENd}       {return END;}
{Do}        {return DO;}
{If}        {return IF;}
{THEn}      {return THEN;}
{ELSe}      {return ELSE;}
";"         {return SEMI;}
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
