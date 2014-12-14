#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "dijkstra.h"
#include "fibonacci_heap.h"
#include "states.h"
#include "utils.h"

#define DEBUG 1

void dijkstra(graph_t* G, int source, int* prev, double* dist){
  fibonacci_heap_t* fh = fibonacciHeapCreate();
  fibonacci_heap_element_t* tmp;
  int u, v, w,  n;
  double d;
  adjacency_list_node_t* edge;

  states_t *states = initStates(G);

  /* for all nodes of the graph s */
  if(DEBUG) {
    printf("Dijkstra: initialisation des distances.\n");
  }
  for (int i = 0; i < G->nb_vertices; i++) {
    if (i == source)
      dist[i] = 0;
    else {
      dist[i] = INFINITY;
    }
    prev[i] = INT_MAX;
  }

  markNode(states, source, VISITED);

  /* add all nodes going out of source to the heap */
  edge = G->adjacency_list_array[source].head;
  n = G->adjacency_list_array[source].nb_members;
  for (int i = 0; i < n; i++) {
    d = dist[source] + edge->weight;
    v = edge->vertex;
    if(DEBUG) {
      printf("Dijkstra: insertion de %d-%d (%f).\n", source, v, d);
    }
    fibonacciHeapInsert(fh, fibonacciHeapNewElement(d, source, v));

    /* next edge */
    edge = edge->next;
  }

  if(DEBUG) {
    printf("Dijkstra: boucle while.\n");
  }
  /* iterate while the heap is not empty */
  while (!fibonacciHeapIsEmpty(fh)) {
    tmp = fibonacciHeapExtractMin(fh);

    u = tmp->from;
    v = tmp->to;
    d = tmp->key;
    free(tmp);
    if(DEBUG) {
      printf("\tExtraction de %d-%d (%f)\n", u, v, d);
    }
    if (!isState(states, v, VISITED)) {
      dist[v] = d;
      prev[v] = u;
      markNode(states, v, VISITED);

      /* explore all edges going out of v */
      n = G->adjacency_list_array[v].nb_members;
      edge = G->adjacency_list_array[v].head;
      for (int i = 0; i < n; i ++) {
        w = edge->vertex;
        if(DEBUG) {
          printf("\t\tSur l'arête %d-%d (%f)\n", v, w, edge->weight);
        }

        d = dist[v] + edge->weight;
        if(DEBUG) {
          printf("\t\t\t: insertion de %d-%d (%f).\n", v, w, d);
        }
        fibonacciHeapInsert(fh, fibonacciHeapNewElement(d, v, w));

        /* next edge */
        edge = edge->next;
      }
    }
  }
  freeStates(states);
  fibonacciHeapFree(fh);
}
