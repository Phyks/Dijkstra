#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

void test_alloc(void* a) {
  if (a == NULL) {
    fprintf(stderr, "Unable to allocate memory.\n");
    exit(EXIT_FAILURE);
  }
}
priority_queue_element_t* priority_queue_extract_min(priority_queue_t* P) {
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
  if (i_min == -1){ 		/* no element in P */
    fprintf(stderr, "P is empty.\n");
    exit(EXIT_FAILURE);
  }

  priority_queue_remove(P, i_min);

  return e_min;
}
void priority_queue_remove(priority_queue_t* P, int x) {
  P->nb_members --;
  for (int i = x; i < P->nb_members; i++) {
    P->elements[i] = P->elements[i+1];
  }
  P->elements = (priority_queue_element_t*)
    realloc(P->elements, sizeof(priority_queue_element_t)*P->nb_members);

  test_alloc(P->elements);
}

void priority_queue_insert(priority_queue_t* P, priority_queue_element_t* e) {
  P->nb_members++;
  P->elements = (priority_queue_element_t*)
    realloc(P->elements, sizeof(priority_queue_element_t)*P->nb_members);
  test_alloc(P->elements);

  P->elements[P->nb_members-1] = *e;
}

int priority_queue_is_empty(priority_queue_t* P) {
  return P->nb_members == 0;
}

void priority_queue_free(priority_queue_t* P) {
  free(P->elements);
  free(P);
}
