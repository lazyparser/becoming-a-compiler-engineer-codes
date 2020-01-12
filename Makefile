default:build check
build:
	flex src/cifa.l
	gcc -o cifa lex.yy.c -lfl
	bison -d src/yufa.y
	flex src/yufa.l
	gcc -o yufa yufa.tab.c lex.yy.c -lfl
check:
	./cifa < test/test.pl0
	./yufa < test/test.pl0
.PHONY:default build check