#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include "utils.h"

void test_alloc(void* a) {
  if (a == NULL) {
    fprintf(stderr, "Unable to allocate memory.\n");
    exit(EXIT_FAILURE);
  }
}
priority_queue_element_t* priorityQueueExtractMin(priority_queue_t* P) {
  int w_min = 0;
  int i_min = -1;
  priority_queue_element_t *e, *e_min;
  for (int i = 0; i < P->nb_members; i++) {
    e = &P->elements[i];
    if (e->edge->weight < w_min) {
      e_min = e;
      i_min = i;
    }
  }
  if (i_min == -1){             /* no element in P */
    fprintf(stderr, "P is empty.\n");
    exit(EXIT_FAILURE);
  }

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
  P->elements = (priority_queue_element_t*)
    safe_realloc(P->elements, sizeof(priority_queue_element_t)*P->nb_members);
}

void priorityQueueInsert(priority_queue_t* P, priority_queue_element_t* e) {
  P->nb_members++;
  P->elements = (priority_queue_element_t*)
    safe_realloc(P->elements, sizeof(priority_queue_element_t)*P->nb_members);

  P->elements[P->nb_members-1] = *e;
}

int priorityQueueIsEmpty(priority_queue_t* P) {
  return P->nb_members == 0;
}

void priorityQueueFree(priority_queue_t* P) {
  free(P->elements);
  free(P);
}
