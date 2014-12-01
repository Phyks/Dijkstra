#include <stdio.h>
#include <stdlib.h>
#include "dijkstra.h"
#include "queue.h"
#include "states.h"
#define UNDEF -1

int* dijkstra(graph_t* G, int s){
  priority_queue_t P;
  priority_queue_element_t tmp;

  adjacency_list_node_t* vertex;
  int u, v, w, n, length;
  adjacency_list_node_t* e;

  int* d_s = (int*) malloc(sizeof(int)*G->nb_vertices);
  states_t *states = initStates(G);

  P.nb_members = 0;
  P.elements = NULL;

  for (int i = 0; i < G->nb_vertices; i++) {
    if (i == s) {
      d_s[i] = 0;
    }
    else {
      d_s[i] = UNDEF;
    }
  }

  markState(states, s, VISITED);

  /* Iterate over all edges going out of s */
  e = G->adjacency_list_array[s].head;
  n = G->adjacency_list_array[s].nb_members;
  for (int i = 0; i < n; i++) {
    length = d_s[s] + e->weight;
    tmp.u = s;
    tmp.edge = e;
    tmp.length = length;
    priorityQueueInsert(&P, &tmp);
    e = e->next;
  }

  while (!priorityQueueIsEmpty(&P)) {
    tmp = *priorityQueueExtractMin(&P);
    u = tmp.u;
    v = tmp.edge->vertex;
    w = tmp.edge->weight;

    if (!isState(states, v, VISITED)) {
      d_s[v] = w;
      markState(states, v, VISITED);

      vertex = G->adjacency_list_array[v].head;
      n = G->adjacency_list_array[v].nb_members;
      for (int i = 0; i < n; i++) {
        length = d_s[u] + vertex->weight;
        tmp.u = u;
        tmp.edge = vertex;
        priorityQueueInsert(&P, &tmp);
        vertex = vertex->next;
      }
    }
  }
  free(vertex);
  free(e);
  return d_s;
}

