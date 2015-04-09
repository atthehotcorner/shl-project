#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ll.h"

ll* llCreate(int x) {
	ll* list = malloc(sizeof(ll));
	list->type = x;
	list->count = 0;
	list->start = NULL;
	list->end = NULL;
	return list;
}

void llFree(ll* list) {
	if (list == NULL) return;

	node* current = list->start;
	node* prev = NULL;

	while (current != NULL) {
		prev = current;
		current = current->next;

		free(prev);
	} 

	free(list);
	list = NULL;
}

void llPush(ll* list, char* name, char* value) {
	if (list == NULL) return;

	// create new node
	node *new = malloc(sizeof(node));
	new->name = name;
	new->value = value;
	new->next = NULL;

	if (list->start == NULL) {
		// no items in list
		list->start = new;
		list->end = new;
	}
	else {
		// add to end
		list->end->next = new;
		list->end = new;
	}

	list->count++;
}

// for alias, dup checking
void llPushAlias(ll* list, char* name, char* value) {
	if (list == NULL) return;

	// current node
	node* current = malloc(sizeof(node));
	current = list->start;

	// traverse list and look if alias already exists
	int nodeReplaced = 0;
	while (current != NULL) {
		if (strcmp(current->name, name) == 0) {
			// overwrite alias
			current->value = value;
			nodeReplaced = 1;
			break;
		}
		current = current->next;
	}

	// fix list pointers
	if (nodeReplaced == 0) {
		// create new node
		node *new = malloc(sizeof(node));
		new->name = name;
		new->value = value;
		new->next = NULL;

		if (list->start == NULL) {
			// no items in list
			list->start = new;
			list->end = new;
		}
		else {
			// add to end
			list->end->next = new;
			list->end = new;
		}
		
		list->count++;
	}

	free(current);
}

// get first found
char* llGet(ll* list, char* name) {
	if (list == NULL) return NULL;

	// current node
	node* current = list->start;

	// traverse list and look for item
	char* value;

	int nodeFound = 0;
	while (current != NULL) {
		if (strcmp(current->name, name) == 0) {
			// item found
			value = current->value;
			nodeFound = 1;
			break;
		}
		current = current->next;
	}

	if (nodeFound == 1) return value;
	else return NULL;
}

// for alias fetching
char* llGetAlias(ll* list, char* name) {
	if (list == NULL) return NULL;

	char* value = llGetAliasRecursive(list, name, name);

	if (strcmp(name, value) == 0) {
		// alias not found or is circular
		return NULL;
	}
	else return value;
}

char* llGetAliasRecursive(ll* list, char* name, char* originName) {
	if (list == NULL) return NULL;

	// current node
	node* current = list->start;

	// traverse list and look for alias
	char* value;

	int nodeFound = 0;
	while (current != NULL) {
		if (strcmp(current->name, name) == 0) {
			// alias found
			value = current->value;
			nodeFound = 1;
			break;
		}
		current = current->next;
	}

	if (nodeFound == 0) {
		// name is not an alias
		return name;
	}
	else if (strcmp(value, originName) == 0) {
		// found alias is circular	
		return originName;
	}
	else {
		return llGetAliasRecursive(list, value, originName);
	}
}

// Remove from front
char* llPop(ll* list) {
	if (list == NULL || list->start == NULL) return NULL;

	// current and prev node
	node* current = list->start;
	node* next = list->start->next;

	char* data = current->name;

	if (next == NULL) {
		// current is an only child
		list->start = NULL;
		list->end = NULL;
	}
	else {
		list->start = next;
	}

	list->count--;
	free(current);
	return data;
}

void llRemove(ll* list, char* name) {
	if (list == NULL) return;

	// current and prev node
	node* current = list->start;
	node* prev = NULL;

	// traverse list and look for item
	while (current != NULL) {
		if (strcmp(current->name, name) == 0) {
			// item found
			if (prev == NULL) {
				// current is start of list
				list->start = current->next;

				if (current->next == NULL) {
					// current is an only child
					list->start = NULL;
					list->end = NULL;
				}
			}
			else if (current->next == NULL) {
				// current is end of list
				prev->next = NULL;
				list->end = prev;
			}
			else {
				// current is in middle of list
				prev->next = current->next;
			}
			
			list->count--;

			free(current);
			break;
		}
		
		prev = current;
		current = current->next;
	}
}

void llPrint(ll* list) {
	if (list == NULL || list->start == NULL) {
		printf("No items found. \n");
		return;
	}
	printf("Number of items: %d \n", list->count);

	// current node
	node* current = list->start;

	// print nodes
	int index = 0;
	
	while (current != NULL) {
		index++;
		if (list->type == 1) printf("%s \n", current->name);
		else printf("%s=%s \n", current->name, current->value);
		current = current->next;
	}
}

