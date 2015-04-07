#ifndef SHELL
#define SHELL

// General
#define true 		1
#define false 		0

#define OK 			0
#define SYSERR 		1

// Colors
#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"
#define RESET "\033[0m"

// Libaries and exterals
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <signal.h>
#include <unistd.h>
#include "linkedlist/ll.h"

/*
  printf(KRED "red\n" RESET);
  printf(KGRN "green\n" RESET);
  printf(KYEL "yellow\n" RESET);
  printf(KBLU "blue\n" RESET);
  printf(KMAG "magenta\n" RESET);
  printf(KCYN "cyan\n" RESET);
  printf(KWHT "white\n" RESET);
  printf(KNRM "normal\n" RESET);
*/

void xsetenv(char* name, char* value);
void xprintenv();
void xunsetenv(char* name);

void xcd(char* path);

char* xgetalias(char* name);
void xsetalias(char* name, char* value);
void xprintalias();
void xunalias(char* name);

void xdebug();
void xbye();

void xexecute(ll* list);
char* xpathlookup(char* command);
void xexecutecommand(ll* list);

// Globals
extern char** environ;
extern char* get_current_dir_name();
extern char* yytext;
ll* aliasTable;

#endif

