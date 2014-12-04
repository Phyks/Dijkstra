#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include "utils.h"

priority_queue_element_t priorityQueueExtractMin(priority_queue_t* P) {
  int d_min = P->elements[0].dist;
  int i_min = -1;
  priority_queue_element_t e, e_min;
  for (int i = 0; i < P->nb_members; i++) {
    e = P->elements[i];
    if (e.dist <= d_min) {
      e_min = e;
      i_min = i;
      d_min = e.dist;
    }
  }
  if (i_min == -1){             /* no element in P */
    fprintf(stderr, "P is empty.\n");
    exit(EXIT_FAILURE);
  }

  /* e_min is copied here, remove its value in P */
  priorityQueueRemove(P, i_min);

  return e_min;
}
void priorityQueueRemove(priority_queue_t* P, int x) {
  P->nb_members --;
  /* shift to the left */
  for (int i = x; i < P->nb_members; i++) {
    P->elements[i] = P->elements[i+1];
  }
  /* shrink the size */
  if (P->nb_members == 0)       /* no element in P */
    P = NULL;
  else {
    P->elements = (priority_queue_element_t*)
      safe_realloc(P->elements,
                   sizeof(priority_queue_element_t)*P->nb_members);
  }
}

void priorityQueueInsert(priority_queue_t* P, priority_queue_element_t e) {
  
  P->nb_members++;
  P->elements = (priority_queue_element_t*)
    safe_realloc(P->elements, sizeof(priority_queue_element_t)*P->nb_members);

  P->elements[P->nb_members-1] = e;
}

void priorityQueueUpdate(priority_queue_t* P, priority_queue_element_t e) {
  int u = e.u;
  int d = e.dist;
  for (int i = 0; i < P->nb_members; i++) {
    if (P->elements[i].u == u) { /* update */
      P->elements[i].dist = d;
      return;
    }
  }
  fprintf(stderr, "Node %d not in P.\n", u);
}

int priorityQueueIsEmpty(priority_queue_t* P) {
  return P->nb_members == 0;
}

void priorityQueueFree(priority_queue_t* P) {
  free(P->elements);
  free(P);
}
