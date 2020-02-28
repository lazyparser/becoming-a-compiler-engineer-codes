default:build check
build:
	flex ./test.l
	gcc -o lex lex.yy.c -lfl

check:
	./lex < ./test.pl0


