#ifndef _NTR_LIST_H_
#define _NTR_LIST_H_ 1
#include <stddef.h>

typedef struct Node Node;

struct Node {
	int value;
	Node *next;
	Node *prev;
};

Node	*llist_create (int val);
void	llist_destroy(Node **ll);
void	llist_add(Node **ll, int val);
int	llist_pop(Node **ll);
int	llist_pops(Node **ll, int n);
int	llist_get(Node *head, int at);
int	llist_getLast(Node *head);
Node 	**llist_getLast_ref(Node **head);
int	llist_index_of(Node *head, int val);

#endif /* ifndef _NTR_LIST_H_ */
