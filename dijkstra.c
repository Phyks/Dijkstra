#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "dijkstra.h"
#include "queue.h"
#include "states.h"
#include "utils.h"

#define DEBUG 0

void dijkstra(graph_t* G, int source, int* prev, double* dist){
  priority_queue_t P;
  priority_queue_element_t tmp;

  int u, v, n;
  double d;
  adjacency_list_node_t* edge;

  states_t *states = initStates(G);


  P.nb_members = 0;
  P.elements   = NULL;
  /* for all nodes of the graph s */
  if(DEBUG) {
    printf("Dijkstra: initialisation des distances.\n");
  }
  for (int i = 0; i < G->nb_vertices; i++) {
    if (i == source) 		/* distance from start point = 0 */
      dist[i] = 0;
    else {
      dist[i] = INFINITY;
    }
    prev[i] = INT_MAX;
    /* add it to the priority queue */
    tmp.u = i;
    tmp.dist = dist[i];
    priorityQueueInsert(&P, tmp); /* insert P <- i, d[i] */
  }
  if(DEBUG) {
    for (int i = 0; i < G->nb_vertices; i++) {
      printf("%d: %f - %d\n", i, dist[i], prev[i]);
    }
  }
  if(DEBUG) {
    printf("terminé.\n");
  }

  while (!priorityQueueIsEmpty(&P)) {
    if(DEBUG) {
      printf("\tNode\tDist\n");
      for (int i = 0; i < P.nb_members; i++) {
        printf("\t%d\t%f\n", P.elements[i].u, P.elements[i].dist);
      }
    }
    tmp = priorityQueueExtractMin(&P);
    u = tmp.u;
    markNode(states, u, VISITED);

    if(DEBUG) {
      printf("\t -> Exploration depuis %d.\n", u);
    }
    /* explore all edges going out of u */
    n = G->adjacency_list_array[u].nb_members;
    edge = G->adjacency_list_array[u].head;

    for (int i = 0; i < n; i ++) {
      v = edge->vertex;

      if(DEBUG) {
        printf("\t\tSur l'arête %d->%d\n", u, v);
      }

      if (!isState(states, v, VISITED)) {
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
  free(P.elements);
}

