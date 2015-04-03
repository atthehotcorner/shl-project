CC=gcc

all: flex-config bison-config build

flex-config:
	flex lexyacc/shell.lex

bison-config:
	bison -dyv lexyacc/shell.y

build:
	$(CC) lex.yy.c y.tab.c shell.c -o shell
