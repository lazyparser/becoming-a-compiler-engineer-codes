default:build check
build:
	flex src/graml.l
	gcc -o gram lex.yy.c -lfl
	bison -d src/parser.y
	flex src/scanner.l
	gcc -o scanner parser.tab.c lex.yy.c -lfl
check:
	./gram < test/test.pl0
	./scanner < test/test.pl0
.PHONY:default build check
