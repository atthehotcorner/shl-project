#ifndef CHAIN
#define CHAIN
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

chain* chainCreate(int x);
void chainReset(chain* command);
void chainPush(chain* command, ll* list);
ll* chainPop(chain* command);
void chainPrint(chain* command);

#endif

