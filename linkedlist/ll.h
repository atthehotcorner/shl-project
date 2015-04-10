typedef struct node {
	char* name;
	char* value;
	struct node* next;
} node;


typedef struct ll {
	// 0 - will use name and value
	// 1 - will only use name
	int type;
	int count;
	struct node* start;
	struct node* end;
} ll;

ll* llCreate(int x);
void llFree(ll* list);

void llPush(ll* list, char* name, char* value);
void llPushAlias(ll* list, char* name, char* value);

char* llGet(ll* list, char* name);
char* llGetAlias(ll* list, char* name);
char* llGetAliasRecursive(ll* list, char* name, char* originName);

void llRemove(ll* list, char* name);
void llPrint(ll* list);
