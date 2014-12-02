#include <stdio.h>
#include <stdlib.h>

#include "dijkstra.h"
#include "queue.h"
#include "states.h"
#include "utils.h"
#define UNDEF -1

void exploreNeighbours(graph_t* G, priority_queue_t* P, int* d_s, int start) {
  adjacency_list_node_t* ngbr;
  int n;
  priority_queue_element_t* tmp;

  /* start -> ngbr */
  ngbr = G->adjacency_list_array[start].head;
  /* n neighbours */
  n = G->adjacency_list_array[start].nb_members;
  for (int i = 0; i < n; i++) {
    /* allocate memory for tmp */
    tmp = (priority_queue_element_t*)
      safe_malloc(sizeof(priority_queue_element_t));
    tmp->u    = start;
    tmp->edge = ngbr;
    tmp->length = d_s[start] + ngbr->weight;
    priorityQueueInsert(P, tmp);
    /* next neighbour */
    ngbr     = ngbr->next;
  }

  free(ngbr);
}

int* dijkstra(graph_t* G, int s){
  priority_queue_t P;
  priority_queue_element_t tmp;

  int u, v, w;

  int* d_s = (int*) safe_malloc(sizeof(int)*G->nb_vertices);
  states_t *states = initStates(G);

  P.nb_members = 0;
  P.elements   = NULL;

  /* for all nodes of the graph s */
  for (int i = 0; i < G->nb_vertices; i++) {
    if (i == s) 		/* distance from start point = 0 */
      d_s[i] = 0;
    else 
      d_s[i] = UNDEF;
  }

  markNode(states, s, VISITED);

  /* Iterate over all edges going out of s */
  exploreNeighbours(G, &P, d_s, s);

  while (!priorityQueueIsEmpty(&P)) {
    tmp = *priorityQueueExtractMin(&P);
    u = tmp.u;
    v = tmp.edge->vertex;
    w = tmp.edge->weight;

    if (!isState(states, v, VISITED)) {
      d_s[v] = w;
      markNode(states, v, VISITED);

      exploreNeighbours(G, &P, d_s, v);
    }
  }
  return d_s;
}

