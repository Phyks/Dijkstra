#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "dijkstra.h"
#include "queue.h"
#include "states.h"
#include "utils.h"

#define UNDEF INT_MAX


void dijkstra(graph_t* G, int source, int* prev, int* dist){
  priority_queue_t P;
  priority_queue_element_t tmp;

  int u, v, n, d;
  adjacency_list_node_t* edge;
    
  states_t *states = initStates(G);

  
  P.nb_members = 0;
  P.elements   = NULL;
  /* for all nodes of the graph s */
  for (int i = 0; i < G->nb_vertices; i++) {
    if (i == source) 		/* distance from start point = 0 */
      dist[i] = 0;
    else {
      dist[i] = UNDEF;

      prev[i] = UNDEF;
    }
    /* add it to the priority queue */
    tmp.u = i;
    tmp.dist = dist[i];
    priorityQueueInsert(&P, tmp); /* insert P <- i, d[i] */
  }

  while (!priorityQueueIsEmpty(&P)) {
    tmp = priorityQueueExtractMin(&P);
    u = tmp.u;
    markNode(states, u, VISITED);

    /* explore all edges going out of u */
    n = G->adjacency_list_array[u].nb_members;
    edge = G->adjacency_list_array[u].head;
    if (n > 0) {                /* if there is an out-
                                   going edge*/
      v = edge->vertex;
    }
    for (int i = 0; i < n; i ++) {
      if (states->states[v] != VISITED) {
	d = dist[u] + edge->weight;
	/* if this path gives a shorter way */
	if (d < dist[v]) {
	  dist[v] = d;
	  prev[v] = u;
	  tmp.u = v;
	  tmp.dist = d;
	  priorityQueueUpdate(&P, tmp);
	}
      }
      /* next neighbour */
      edge = edge->next;
    }
  }
  freeStates(states);
}

