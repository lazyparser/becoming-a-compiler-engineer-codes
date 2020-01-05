all:
	flex src/scanner.l
	gcc lex.yy.c -lfl
	./a.out < test/testcase.pl0
