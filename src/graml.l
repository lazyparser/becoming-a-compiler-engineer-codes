%{
int numChars = 0, numWords = 0, numLines = 0,numInts = 0,numOps = 0;
%}

WORD		[a-zA-Z][a-zA-Z0-9]*
INT		[0-9]+
OP		["+"|"-"|"*"|"/"|":="|"<"|"<="|">"|">="]

%%


\n		{ numLines++; numChars++; }
{WORD}		{ numWords++; numChars += yyleng; return 1; }
{INT}		{ numInts++; numChars += yyleng; return 1; }
{OP}		{ numOps++; numChars += yyleng; return 1; }
<<EOF>>		{ return 0; }
.		{ numChars++; }



%%

int main() {
	int token_type;
	while (token_type = yylex()) {
		printf("Item:\t%s\n", yytext);
	}
	printf("\nChars\tWords\tInts\tOp\tLines\n");
	printf("%d\t%d\t%d\t%d\t%d\n", numChars, numWords, numInts, numOps, numLines);
	return 0;
}

int yywrap() {
	return 1;
}
