CC=gcc

all: flex-config bison-config build

flex-config:
	flex lexyacc/shell.lex

bison-config:
	bison -dyv lexyacc/shell.y

build:
	$(CC) lex.yy.c y.tab.c linkedlist/ll.c shell.c -g -o shell
