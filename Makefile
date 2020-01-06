all:
	flex src/scanner.l
	gcc lex.yy.c -lfl
	./a.out < test/testcase.pl0
	bison -d src/gram.y
	flex src/gram.l
	gcc gram.tab.c lex.yy.c -lfl
	./a.out
