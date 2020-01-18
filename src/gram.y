%{
void yyerror(const char* msg) {}
int yylex(void);
%}

%token ID INT PLUS ASSIGN

%%

S : E 

E : ID ASSIGN INT
   | ID ASSIGN ID PLUS INT   

%%



int main(){

return yyparse();

}