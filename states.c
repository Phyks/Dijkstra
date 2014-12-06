#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "states.h"
#include "utils.h"

/**
 * Init states array.
 */
states_t *initStates(graph_t const *graph) {
  states_t *states = (states_t *) safe_malloc(sizeof(states_t));

  if (states == NULL) {
    fprintf(stderr, "Unable to allocate memory to store states.\n");
    exit(EXIT_FAILURE);
  }

  states->nb_members = graph->nb_vertices;
  states->states = safe_malloc(sizeof(state_t) * states->nb_members);

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
void freeStates(states_t *states) {
  free(states->states);
  free(states);
}


/**
 * Mark a node with a given state.
 */
void markNode(states_t *states, int v, state_t state) {
  states->states[v] = state;
}


/**
 * Check if the node `v` is already of given state.
 */
int isState(states_t const *states, int v, state_t state) {
  return (states->states[v] == state);
}


/**
 * Check if *a* node is already of given state.
 */
int someIsState(states_t const *states, state_t state) {
  for (int i = 0; i < states->nb_members; i++) {
    if (states->states[i] == state) {
      return true;
    }
  }
  return false;
}
