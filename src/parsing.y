%{
/*语法树节点形式name x val y,
  name:终结符或非终结符
  x:符号的编号
  y:符号属性值
*/
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
int yylex(void);
void yyerror(char *S);
char *c_temp;
int i_temp;
int add_n=0,sub_n=0;
int mul_n=0,div_n=0;
int exp_n=0,factor_n=0,cell_n=0,number_n=0;
%}

%union {int num; char *id;}
%token <id> ID NUMBER
%token ASSIGN ADD SUB MUL DIV
%type <num> exp factor cell
%start prog

%%
prog : stm;

stm : ID ASSIGN exp {

                    }
    ;
exp : factor {$$ = $1;
              printf("exp%dval%d -> factor%dval%d;\n",exp_n,$$,factor_n++,$1);
             }
     |exp ADD factor {
                      $$ = $1 + $3;
                      //printf("there is an add\n");
                      printf("exp%dval%d -> exp%dval%d;\n",exp_n,$1 + $3,exp_n++,$1);
                      printf("exp%dval%d -> ADD%d;\n",exp_n,$1+$3,add_n++);
                      printf("exp%dval%d -> factor%dval%d;\n",exp_n,$1+$3,factor_n++,$3);
                     }
     |exp SUB factor {
                      $$ = $1 - $3;
                      printf("exp%dval%d -> exp%dval%d;\n",exp_n,$1 - $3,exp_n++,$1);
                      printf("exp%dval%d -> SUB%d;\n",exp_n,$1 - $3,sub_n++);
                      printf("exp%dval%d -> factor%dval%d;\n",exp_n,$1 - $3,factor_n++,$3);
                     }
     ;
factor : factor MUL cell {
                         $$=$1 * $3;
                         printf("factor%dval%d -> factor%dval%d;\n",factor_n,$1*$3,factor_n++,$1);
                         printf("factor%dval%d -> MUL%d;\n",factor_n,$1*$3,mul_n++);
                         printf("factor%dval%d -> cell%dval%d;\n",factor_n,$1*$3,cell_n++,$3);
                        }
        |factor DIV cell {
                         $$=$1 / $3;
                         printf("factor%dval%d -> factor%dval%d;\n",factor_n,$1/$3,factor_n++,$1);
                         printf("factor%dval%d -> DIV%d;\n",factor_n,$1/$3,div_n++);
                         printf("factor%dval%d -> cell%dval%d;\n",factor_n,$1/$3,cell_n++,$3);
                        }
        |cell {
               $$=$1;
               printf("factor%dval%d -> cell%dval%d;\n",factor_n,$$,cell_n++,$1);
              }
        ;
cell : NUMBER {
                 c_temp=$1; i_temp=atoi(c_temp);
                 $$ = i_temp;
                 printf("cell%dval%d -> NUMBER%dval%s;\n",cell_n,$$,number_n++,$1);
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
