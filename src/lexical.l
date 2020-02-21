%{
//部分内容引用自：Andrew W.Appel. 现代编译原理C语言描述[M]. 北京:人民邮电出版社，2006:20-21
int row_num = 1, col_num = 1;
#define ADJ (col_num+=yyleng)
%}
KEYWORDS   ["CONST"|"VAR"|"PROCEDURE"|"CALL"|"BEGIN"|"END"|"IF"|"THEN"|"WHILE"|"DO"|"ODD"|"const"|"var"|"procedure"|"call"|"begin"|"end"|"if"|"then"|"while"|"do"|"odd"]
OP         ["+"|"*"|"/"|"-"|":"|"="|"<"|">"|"#"]
WORD       [a-zA-Z]+ 
NUMB       [0-9]+
DELI       [";"]
%% 

{KEYWORDS}                     {printf("(KEYWORDS: %s)\n", yytext);ADJ;}
{NUMB}                         {if (yyleng > 14)
                                    {
                                    printf("error!(%d,%d)\n",row_num,col_num);ADJ;
                                    }
                                else{
                                    printf("(NUMB,%s)\n",yytext);ADJ;
                                    }}
{WORD}                         {printf("(WORD,%s)\n",yytext);ADJ;}
{OP}                           {printf("(OPER,%s)\n",yytext);ADJ;}
{DELI}                         {printf("(DELI,%s)\n",yytext);ADJ;}
%%

