%{
#include <stdio.h>
int yylex(void);
%}

	
%token	MUL DIV EQUAL EOL NUMBER
 

%%

S:
 |S B EOL
 ;
B:
 |B MUL F
 ;
F:
 |F DIV T
 ;
T:NUMBER
 |EQUAL B
 ;

%%
main(int argc,char **argv)
{
  yyparse();
}
yyerror(char *s)
{
  fprintf(stderr, "error: %s\n", s);
}

