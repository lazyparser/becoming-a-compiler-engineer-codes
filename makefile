default:build check

build:

	flex src/cifa.l

	gcc -o cifa lex.yy.c -lfl

	bison -d src/yufa2.y

	flex src/yufa.l

	gcc -o yufa yufa2.tab.c lex.yy.c -lfl

check:

	./cifa < test/testcase.pl0

	./yufa < test/testcase.pl0

.PHONY:default build check
