#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "dijkstra.h"
#include "fibonacci_heap.h"
#include "states.h"
#include "utils.h"

#define DEBUG 0

void dijkstra(graph_t* G, int source, int* prev, double* dist){
  fibonacci_heap_t* fh = fibonacciHeapCreate();
  fibonacci_heap_element_t* tmp;
  int u, v, n;
  double d;
  adjacency_list_node_t* edge;

  states_t *states = initStates(G);

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
    fibonacciHeapInsert(fh,
                        fibonacciHeapNewElement(dist[i], i));
  }


  while (!fibonacciHeapIsEmpty(fh)) {
    tmp = fibonacciHeapExtractMin(fh);
    u = tmp->vertex;
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
	  fibonacciHeapInsert(fh,
                              fibonacciHeapNewElement(v, d));
	}
      }
      /* next neighbour */
      edge = edge->next;
    }
  }
  freeStates(states);
  fibonacciHeapFree(fh);
}
