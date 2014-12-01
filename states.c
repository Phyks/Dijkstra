#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "states.h"

/**
 * Init states array.
 */
struct states_t *initStates(graph_t *graph) {
  struct states_t *states = (struct states_t *) malloc(sizeof(struct states_t));

  if (states == NULL) {
    fprintf(stderr, "Unable to allocate memory to store states.\n");
    exit(EXIT_FAILURE);
  }

  states->nb_members = graph->nb_vertices;
  states->states = malloc(sizeof(state_t) * states->nb_members);

  if (states->states == NULL) {
    fprintf(stderr, "Unable to allocate memory to store states.\n");
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < states->nb_members; i++) {
    states->states[i] = NOT_VISITED;
  }

  return states;
}


/**
 * Free the states array.
 */
void destroyStates(struct states_t *states) {
  free(states);
}


/**
 * Mark a node with a given state.
 */
void markNode(struct states_t *states, int v, state_t state) {
  states->states[v] = state;
}


/**
 * Check if node is already of given state.
 */
int isState(struct states_t* states, int v, state_t state) {
  for (int i = 0; i < states->nb_members; i++) {
    if (states->states[i] == state) {
      return true;
    }
  }
  return false;
}
