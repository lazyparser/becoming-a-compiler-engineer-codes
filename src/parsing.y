%{
    #include <stdio.h>
    int yylex(void);
%}

%token     ID NUMBER ADD SUB MUL DIV EQ EOL

%%
S:
 |S T EOL 
 ;
T: 
 |T ADD F 
 |T SUB F 
 ;
F: 
 |F MUL B 
 |F DIV B 
 ;
B: NUMBER 
 |EQ B 
; 
%%
int main()
{
    return yyparse();
}
yyerror(char *s)
{
    fprintf(stderr,"error:%s\n",s);
}
