DIG    [0-9]+
OPE    ["+"|"-"|"*"|"/"|":"|"="|"!"|">"|"<"]
LIN    [\n]
%{
	enum yytokentype{
	    DIGI = 258,
	    OPER = 259,
	    LINE = 260,
	};
	int yylval;
%}
%%

[0-9]+	{ return DIGI; }
["+"|"-"|"*"|"/"|":"|"="|"!"|">"|"<"]	{ return OPER; }
\n	{ return LINE; }
[ \t]	{ /* 忽略空白字符 */ }
.	{ printf("Mystery character %c\n", *yytext); }
