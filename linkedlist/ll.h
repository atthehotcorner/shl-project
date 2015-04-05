typedef struct node {
	char* name;
	char* value;
	struct node* next;
} node;


typedef struct ll {
	struct node* start;
	struct node* end;
} ll;

ll* llCreate();
void llFree(ll* list);
void llPush(ll* list, char* name, char* value);
char* llGet(ll* list, char* name);
char* llGetRecursive(ll* list, char* name, char* originName);
void llRemove(ll* list, char* name);
void llPrint(ll* list);

