default:build check
build:
	flex src/graml.l
	cc lex.yy.c -lfl
	./a.out < test/test.pl0
	bison -d src/parser.y
	flex src/scanner.l
	gcc parser.tab.c lex.yy.c -lfl
	./a.out < test/test.pl0
check:
.PHONY:default build check
