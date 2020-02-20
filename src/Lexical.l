%{
//部分代码引用：Andrew W.Appel. 现代编译原理C语言描述[M]. 北京:人民邮电出版社,2006:20-21
int row_num = 1, col_num = 1;
#define ADJ  (col_num+=yyleng)
%}
digits    [0-9]+
ID        [A-Za-z][A-Za-z0-9]+
keywords         	["CONST"|"VAR"|"PROCEDURE"|"CALL"|"BEGIN"|"END"|"IF"|"THEN"|"WHILE"|"DO"|"ODD"|"const"|"var"|"procedure"|"call"|"begin"|"end"|"if"|"then"|"while"|"do"|"odd"]
OP        ["+"|"-"|"*"|"/"|"#"|"<"|">"|"="|"<>"|"!="|">="|"<="|":="]
Del       ["("|")"|","|":"|";"|"."]
%%
{ID}         {printf("An identifier: %s\n",yytext);ADJ;}
{digits}     {if(yyleng > 14){
		printf("Error!Expected a shorter identifier!(%d,%d)\n",row_num,col_num);ADJ;}
              else{printf("An integer: %s\n",yytext);ADJ;}}
{Del}        {printf("An Delimiter:%s\n",yytext);ADJ;}
"\n"         {++row_num;}
{keywords}   {printf("A keyword: %s\n",yytext);ADJ;}
{OP}         {printf("A operator: %s\n",yytext);ADJ;}

