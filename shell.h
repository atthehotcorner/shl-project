// General
#define OK 		0
#define SYSERR 		1
#define MAX_LENGTH	1024

// For do_it switch
#define SETENV		1
#define PRINTENV	2
#define UNSETENV	3
#define CD 		4
#define ALIAS 		5
#define UNALIAS 	6
#define LS		7
#define PWD 		8
#define DEBUG		9
#define BYE 		10

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

// Globals
extern char** environ;
extern char* get_current_dir_name();
int builtin;
int CMD;

char* PATH;
char* HOME;

char* setenvName;
char* setenvValue;
char* unsetenvName;

char* cdPath;

char* aliasName;
char* aliasValue;
char* unaliasName;


