#ifndef _QUEUE_H_
#define _QUEUE_H_

#include <stdlib.h>
#include "graph.h"

typedef struct priority_queue_element {
  int u;
  adjacency_list_node_t* edge;
  int length;
} priority_queue_element_t;
  
typedef struct priority_queue {
  int nb_members;
  priority_queue_element_t* elements;
} priority_queue_t;

priority_queue_element_t* priority_queue_extract_min(priority_queue_t* P) ;
void priority_queue_remove(priority_queue_t* P, int x) ;
void priority_queue_insert(priority_queue_t* P, priority_queue_element_t* e);
int priority_queue_is_empty(priority_queue_t* P);
void priority_queue_free(priority_queue_t* P);

#endif
