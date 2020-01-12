KEYWORDS   ["CONST"|"VAR"|"PROCEDURE"|"CALL"|"BEGIN"|"END"|"IF"|"THEN"|"WHILE"|"DO"|"ODD"|"const"|"var"|"procedure"|"call"|"begin"|"end"|"if"|"then"|"while"|"do"|"odd"]
OP         ["+"|"*"|"/"|"-"|":"|"="|"<"|">"|"#"]
WORD       [a-zA-Z]+ 
NUMB       [0-9]+
DELI       [";"]
%% 

{KEYWORDS}                     {printf("(KEYWORDS: %s)\n", yytext);}
{WORD}                         {printf("(WORD,%s)\n",yytext);}
{NUMB}                         {printf("(NUMB,%s)\n",yytext);}
{OP}                           {printf("(OPER,%s)\n",yytext);}
{DELI}                         {printf("(DELI,%s)\n",yytext);}
%%

