#include "ll.h"

typedef struct chain {
	int count;
	int background; // 0 - not in background, 1 - in bg
	
	char* fileIn;
	char* fileOut;
	char* fileErrorOut;

	struct ll* start;
	struct ll* end;
} chain;

ll* llCreate(int x);
void llFree(ll* list);

void llPush(ll* list, char* name, char* value);
void llPushAlias(ll* list, char* name, char* value);

char* llGet(ll* list, char* name);
char* llGetAlias(ll* list, char* name);
char* llGetAliasRecursive(ll* list, char* name, char* originName);

char* llPop(ll* list);
void llRemove(ll* list, char* name);
void llPrint(ll* list);

