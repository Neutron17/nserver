#include "llist.h"
#include <stdlib.h>
#include "log.h"
#include "global.h"

Node *llist_create(int val) {
	Node *ret = malloc(sizeof(Node));
	ret->value = val;
	ret->next = NULL;
	ret->prev = NULL;
	return ret;
}
void llist_destroy(Node **head) {
	if(head == NULL)
		return;
	Node *node = *head;
	Node *next;
	while(node->next != NULL) {
		next = node->next;
		free(node);
		node = next;
	}
	//free(head);
	/*for(int i = 0; i < head->used; i++) {
		free(node);
		node = next;
		next = node->next;
	}*/
}
int llist_get(Node *head, int at) {
	if(at == 0)
		return head->value;
	Node *node = head;
	int i = 0;
	for(; i <= at; i++) {
		if(node == NULL)
			return -1;
		node = node->next;
	}
	if(node == NULL)
		return -2;
	return i;
}
int llist_index_of(Node *head, int val) {
	Node *node = head;
	int i = 0;
	if(node->value == val)
		return i;
	while(node->next != NULL && node->next->value != val) {
		node = node->next;
		i++;
	}
	if(node->next == NULL)
		return -1;
	return node->next->value;
}

int llist_getLast(Node *head) {
	Node *last = head;
	while(last->next != NULL)
		last = last->next;
	return last->value;
}
Node **llist_getLast_ref(Node **head) {
	Node **last = head;
	while((*last)->next != NULL)
		*last = (*last)->next;
	return last;
}

void llist_add(Node **head, int val) {
	//printf("Add: %d\n", llist_getLast(*head));
	Node *node = malloc(sizeof(Node));
	node->value = val;
	node->next = *head;
	node->prev = NULL;
	(*head)->prev = node;
	(*head) = node;
}
int llist_pop(Node **head) {
	if(*head == NULL)
		return -1;
	Node *tmp = *head;
	*head = (*head)->next;
	int ret = tmp->value;
	free(tmp);
	return ret;
}
int llist_pops(Node **head, int n) {
	int ret;
	for(int i = 0; i < n; i++)
		ret = llist_pop(head);
	return ret;
}

