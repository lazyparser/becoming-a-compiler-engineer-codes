%{
    #include <stdio.h>
    int yylex(void);
%}


%token     ID ASSIGN WHILE BEGIn END DO IF THEN ELSE SEMI NUMBER ADD SUB MUL DIV EOL


%%

stm :
    |ID ASSIGN ID
    |WHILE ID DO stm
    |BEGIn stmlist END
    |IF ID THEN stm
    |IF ID THEN stm ELSE stm
    |MATH
    ;
stmlist :
        |stm
        |stmlist SEMI stm
        ;
MATH :
     |MATH TERM EOL
     ;
TERM :
     |TERM ADD FACTOR
     |TERM SUB FACTOR
     ;
FACTOR :
       |FACTOR MUL CELL
       |FACTOR DIV CELL
       ;
CELL :NUMBER
     |ASSIGN CELL
     ;
%%
int main()
{
    return yyparse();
}
yyerror(char *s)
{
    printf(stderr,"error:%s\n",s);
}
