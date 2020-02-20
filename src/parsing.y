%{
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
int yylex(void);
void yyerror(char *S);
char *c_temp;
int i_temp;
%}

%union {int num; char *id;}
%token <id> ID NUMBER
%token ASSIGN ADD SUB MUL DIV
%type <num> exp
%start prog

%%
prog : stm;

stm :ID ASSIGN exp {
                    //printf("ID%s := %d\n",$1,$3);
                   }
    ;
exp :NUMBER {//printf("there is a define\n");
             c_temp=$1;i_temp=atoi(c_temp);
             $$ = i_temp;
             printf("exp%d -> NUMBER%s;\n",$$,$1);
            }
    |exp ADD exp {
                 $$ = $1 + $3;
                 //printf("there is an add\n");
                 printf("exp%d -> exp%d;\n",$1 + $3, $1);
                 printf("exp%d -> ADD;\n",$1+$3);
                 printf("exp%d -> exp%d;\n",$1+$3,$3);
                 }
    |exp SUB exp {
                  $$ = $1 - $3;
                  printf("exp%d -> exp%d;\n",$1 - $3, $1);
                  printf("exp%d -> SUB;\n",$1 - $3);
                  printf("exp%d -> exp%d;\n", $1 - $3, $3);
                  }
    |exp MUL exp {
                         $$=$1*$3;
                         printf("exp%d -> exp%d;\n",$1*$3,$1);
                         printf("exp%d -> MUL;\n",$1*$3);
                         printf("exp%d -> exp%d;\n",$1*$3,$3);
                        }
    |exp DIV exp {
                   $$=$1*$3;
                   printf("exp%d -> exp%d;\n",$1*$3,$1);
                   printf("exp%d -> DIV;\n",$1*$3);
                   printf("exp%d -> exp%d;\n",$1*$3,$3);
                  }
    ;
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
void yyerror(char *s)
{
    printf("error:%s!\n",s);
}
