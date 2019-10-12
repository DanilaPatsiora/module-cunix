#include "linked_list.h"
	#include <stddef.h>
	#include <stdio.h>
	#include <stdlib.h>
	
	
	
	node_t  *list_create(char *key, char *value)
	{
	  node_t *tmp;
	  tmp=malloc((sizeof(node_t)));
	  tmp->key=key;
	  tmp->value=value;
	  tmp->next=NULL;
	  return tmp;
	}
	void list_destroy(node_t **head)
		{
		 if ((*head) == NULL)
		     return;
		 node_t *next;
		 for (node_t *current =(*head); current; current = next)
		    {
		        next = current->next;
	          free(current);
		    }
		 (*head) = NULL;
		 }
	
	
	void    list_push(node_t *head, char *key, char* value)
	{
	  node_t *tmp=head;
	  for(;tmp->next!=NULL;tmp=tmp->next);
	  tmp->next=malloc(sizeof(node_t));
	  tmp->next->key=key;
	tmp->next->value=value;
	  tmp->next->next=NULL;
	
	}
	
	
	
	void list_print(node_t *head)
	    {
	      node_t   *tmp = head;
	      while(tmp !=  NULL)
	      {
	        printf("%s : %s\n", tmp->key, tmp->value);
	        tmp = tmp->next;
	      }
	   }
   void list_print_pair(node_t *head, char* key)
            {
              node_t   *tmp = head;
              while(tmp->key !=  key)
              {
                tmp = tmp->next;
              }
	    printf("%s : %s\n",key, tmp->value);
           }
