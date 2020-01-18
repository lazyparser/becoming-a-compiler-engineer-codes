default: build check

build:
	flex src/scanner.l
	gcc -o scanner lex.yy.c -lfl
	bison -d src/gram.y
	flex src/gram.l
	gcc -o gram gram.tab.c lex.yy.c -lfl

check:
	./scanner < test/testcase01.pl0
	./gram < test/testcase02.pl0


.PHONY: default build check
