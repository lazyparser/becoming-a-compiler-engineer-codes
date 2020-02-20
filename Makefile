default:build check
build:
	flex src/Lexical.l
	gcc -o lexical lex.yy.c -lfl
	bison -d src/parsing.y
	flex src/parsing.l
	gcc -o parsing parsing.tab.c lex.yy.c -lfl
check:
	./lexical < test/test.pl0
	./parsing < test/test1
	dot -Tpdf graph.txt -o test1_pdf.pdf
.PHONY:default build check
