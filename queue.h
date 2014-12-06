#ifndef _QUEUE_H_
#define _QUEUE_H_

#include <stdlib.h>
#include "graph.h"

typedef struct priority_queue_element {
  int u;
  double dist;
} priority_queue_element_t;

typedef struct priority_queue {
  int nb_members;
  priority_queue_element_t* elements;
} priority_queue_t;

priority_queue_element_t priorityQueueExtractMin(priority_queue_t* P) ;
void priorityQueueRemove(priority_queue_t* P, int x) ;
void priorityQueueInsert(priority_queue_t* P, priority_queue_element_t e);
void priorityQueueUpdate(priority_queue_t* P, priority_queue_element_t e);
int priorityQueueIsEmpty(priority_queue_t* P);
void priorityQueueFree(priority_queue_t* P);

#endif
