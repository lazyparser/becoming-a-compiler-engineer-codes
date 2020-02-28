

KeyWord	begin|call|const|do|end|if|odd|procedure|read|then|var|while|write
ID      [A-Za-z][A-Za-z0-9]*
NUM	[0-9]+
OP 	["+"|"-"|"*"|"/"]
Del	["("|")"|","|";"|"."|"#"]
ASIGN 	:=


%%

{KeyWord}	printf("A KeyWord: %s\n",yytext);
{ID}	printf("A ID: %s\n",yytext);
{NUM}	printf("A NUM: %s\n",yytext);
{OP}	printf("An operator: %s\n",yytext);
{Del}	printf("A Del: %s\n",yytext);
{ASIGN}    printf("An ASIGN: %s\n",yytext);

[ \t\n]+ 
%%

int yywrap(void) { 
    return 1;
}

