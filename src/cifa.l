%{
	#include<stdio.h>
	#include<stdlib.h>
	#include<string.h>;
%}
LETTER            [a-zA-Z]+ 
ID                [a-zA-Z][a-zA-Z0-9]*
OPT               ["+"|"-"|"*"|"/"|":"|"="|"!"|">"|"<"]
DIGIT             [0-9]+ 
KEYWORDS["CONST"|"VAR"|"PROCEDURE"|"CALL"|"BEGIN"|"END"|"IF"|"THEN"|"WHILE"|"DO"|"ODD"|"const"|"var"|"procedure"|"call"|"begin"|"end"|"if"|"then"|"while"|"do"|"odd"]
%%
{KEYWORDS}         {printf("line%d:(keyword, %s)\n",line,yytext);}
{DIGIT}           {printf("line%d:(integer, %s)\n",line,yytext);} 
{ID}              {printf("line%d:(identify, %s)\n",line,yytext);} 
{OPT}             {printf("line%d:(OPT, %s)\n",line,yytext);}
["\t"]+            {}

%%
int main(){
	yyin=fopen("F:/data.txt","r");
	yylex();
	return 0;
}
int yywrap(){
	return 1;
}
