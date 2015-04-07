CC = gcc
LEX = flex
YACC = bison

all: lex-config yacc-config build

lex-config:
	 $(LEX) lexyacc/shell.lex

yacc-config:
	 $(YACC) -dyv lexyacc/shell.y

build:
	$(CC) lex.yy.c y.tab.c linkedlist/ll.c shell.c -g -o shell
