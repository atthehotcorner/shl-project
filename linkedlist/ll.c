#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ll.h"

ll* llCreate() {
	ll* list = malloc(sizeof(ll));
	list->start = NULL;
	list->end = NULL;
	return list;
}

void llFree(ll* list) {
	node* current = list->start;
	node* prev = NULL;

	while (current != NULL) {
		prev = current;
		current = current->next;

		free(prev);
	} 

	free(list);
}

void llPush(ll* list, char* name, char* value) {
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
	}

	free(current);
}

char* llGet(ll* list, char* name) {
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

	/* look for alias in alias, return NULL if infinite
	if (nodeFound == 1) {
		// code here plz
	}*/

	if (nodeFound == 1) return value;
	else return NULL;
}

void llRemove(ll* list, char* name) {
	// current and prev node
	node* current = list->start;
	node* prev = NULL;

	// traverse list and look for alias
	while (current != NULL) {
		if (strcmp(current->name, name) == 0) {
			// alias found
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

			free(current);
			break;
		}
		
		prev = current;
		current = current->next;
	}
}

void llPrint(ll* list) {
	// current node
	node* current = list->start;

	// print nodes
	int index = 0;
	while (current != NULL) {
		index++;
		printf("%s=%s \n", current->name, current->value);
		current = current->next;
	}

	if (index == 0) printf("No items found. \n");
}

