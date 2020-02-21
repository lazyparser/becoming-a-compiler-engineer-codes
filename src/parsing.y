%{
    #include <stdio.h>
    #include <string.h>
    #include <stdlib.h>
    int yylex(void);
    void yyerror(char *S);
    char *c_temp;
    int i_temp;
    int A_count=0;S_count=0;
    int M_count=0;D_count=0;
    int E_count=0;F_count=0;C_count=0;N_count=0;
%}

%union     {int num; char *id;}
%token     <id> ID NUMBER 
%token     ADD SUB MUL DIV EQ
%type      <num> E F C
%start     ST 

%%
ST: st;

st:
 ID EQ E; 
 ;
E: F {$$ = $1;
                printf("%d -> %d;\n",$$,$1);} 
 |E ADD F {$$ = $1 + $3;
                printf("%d -> %d;\n",$$, $1);
                printf("%d -> ADD;\n",$$);
                printf("%d -> %d;\n",$$,$3);}
 |E SUB F {$$ = $1 - $3;
                printf("%d -> %d;\n",$$, $1);
                printf("%d -> SUB;\n",$$);
                printf("%d -> %d;\n",$$, $3);}
  ;
F: F MUL C {$$=$1*$3;
                printf("%d -> %d;\n",$$,$1);
                printf("%d -> MUL;\n",$$);
                printf("%d -> %d;\n",$$,$3);}
   |F DIV C {$$=$1*$3;
                printf("%d -> %d;\n",$$,$1);
                printf("%d -> DIV;\n",$$);
                printf("%d -> %d;\n",$$,$3);}
   |C {$$=$1;
                printf("%d -> %d;\n",$$,$1);}
    ; 
C: NUMBER{      c_temp=$1; i_temp=atoi(c_temp);
                $$ = i_temp;
                printf("%d -> NUMBER%s;\n",$$,$1);}
%%
int main()
{
    freopen("graph.txt","w",stdout);
    printf("digraph ast{\n");
    printf("node [shape = circle, fontsize = 10];\n");
    printf("node [shape=\"plaintext\"];\n");
    yyparse();
    printf("}");
    fclose(stdout);
    return 0;
}
yyerror(char *s)
{
    fprintf(stderr,"error:%s\n",s);
}
