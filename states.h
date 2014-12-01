#ifndef _STATES_H_
#define _STATES_H_

#include "graph.h"

typedef enum state {
  NOT_VISITED,
  VISITING,
  VISITED
} state_t;

struct states_t {
  int nb_members;
  state_t *states;
};


struct states_t *initStates(graph_t *graph);
void destroyStates(struct states_t *states);
void markNode(struct states_t *states, int v, state_t state);
int isState(struct states_t* states, int v, state_t state);

#endif  // _STATES_H_
