%{
#include <stdio.h>
#define YYSTYPE char *
void yyerror(const char* msg) {}
int yylex(void);
%}

%start P
%token ID INT ASSIGN

%%

P: St P
  |
  ;

St: ID ASSIGN Exp   { printf("%s assign ", $1); $1=$3; printf("%s\n", $3); }
   ;

Exp:  INT { $$=$1; printf("num is %s\n", $1); } 
      ;


%%


int main(){

return yyparse();

}