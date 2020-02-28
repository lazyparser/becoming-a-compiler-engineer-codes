%{
#include <stdio.h>
void yyerror(const char* msg) {}
int yylex(void);
%}

%start S 
%token NUM
%left OP


%%

S   :   S P '\n'        { printf("Result = %d\n", $2);printf("输入“.”可以结束计算\n"); }
    |   
    ;

P   :   P '+' P         { $$ = $1 + $3; }
    |   P '-' P         { $$ = $1 - $3; }
    |   P '*' P         { $$ = $1 * $3; }
    |   P '/' P         { $$ = $1 / $3; }
    |   NUM           { $$ = $1; }
    |   '(' P ')'       { $$ = $2; }
    ;

%%

int main() {
    return yyparse();
}