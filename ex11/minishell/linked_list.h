#ifndef   LINKED_LIST_H_
# define  LINKED_LIST_H_

typedef struct  node {
    char        *key;
	char *value;
    struct node *next;
}              node_t;

node_t  *list_create(char *key, char *value);

void    list_destroy(node_t **head);

void    list_push(node_t *head, char *key, char *value);

void list_print_pair(node_t *head, char* key);
void    list_print(node_t *head);

#endif /* LINKED_LIST_H_ */
