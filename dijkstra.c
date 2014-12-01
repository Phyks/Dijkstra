#include <stdio.h>
#include <stdlib.h>
#include "dijkstra.h"
#include "queue.h"
#define UNDEF -1

int* dijkstra(graph_t* G, int s){
  priority_queue_t P;
  priority_queue_element_t tmp;

  adjacency_list_node_t* vertex;
  int u, v, w, n, length;
  adjacency_list_node_t* e;

  int* d_s = (int*) malloc(sizeof(int)*G->nb_vertices);
  seen_t seen;

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

  mark_seen(&seen, s);

  /* Iterate over all edges going out of s */
  e = G->adjacency_list_array[s].head;
  n = G->adjacency_list_array[s].nb_members;
  for (int i = 0; i < n; i++) {
    length = d_s[s] + e->weight;
    tmp.u = s;
    tmp.edge = e;
    tmp.length = length;
    priority_queue_insert(&P, &tmp);
    e = e->next;
  }

  while (!priority_queue_is_empty(&P)) {
    tmp = *priority_queue_extract_min(&P);
    u = tmp.u;
    v = tmp.edge->vertex;
    w = tmp.edge->weight;

    if (!is_seen(&seen, v)) {
      d_s[v] = w;
      mark_seen(&seen, v);

      vertex = G->adjacency_list_array[v].head;
      n = G->adjacency_list_array[v].nb_members;
      for (int i = 0; i < n; i++) {
        length = d_s[u] + vertex->weight;
        tmp.u = u;
        tmp.edge = vertex;
        priority_queue_insert(&P, &tmp);
        vertex = vertex->next;
      }
    }
  }
  free(vertex);
  free(e);
  return d_s;
}

