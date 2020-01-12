%{

#include <stdio.h>

int yylex(void);

%}



	

%token	ADD SUB MUL DIV ABS EOL DIGIT LETTER 
 



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

T:DIGIT

 |ABS B

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
