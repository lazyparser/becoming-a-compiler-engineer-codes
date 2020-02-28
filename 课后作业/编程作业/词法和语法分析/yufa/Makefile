default:build
build:
	bison -d ./yufa.y
	flex ./yufa.l
	gcc -o yufa yufa.tab.c lex.yy.c -lfl
	./yufa