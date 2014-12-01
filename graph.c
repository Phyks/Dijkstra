#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "graph.h"
#include "states.h"


/**
 * Create an empty graph with n vertices
 */
graph_t *createGraph(int n) {
  graph_t *graph = (graph_t *) malloc(sizeof(graph_t));

  if(graph == NULL) {
    fprintf(stderr, "Unable to allocate memory for graph.\n");
    exit(EXIT_FAILURE);
  }

  graph->nb_vertices = n;

  /* Create an array of adjacency lists*/
  graph->adjacency_list_array = (adjacency_list_t *) malloc(n * sizeof(adjacency_list_t));
  if(!graph->adjacency_list_array) {
    fprintf(stderr, "Unable to allocate memory for adjacency list array.\n");
    exit(EXIT_FAILURE);
  }

  for(int i = 0; i < n; i++) {
    graph->adjacency_list_array[i].head = NULL;
    graph->adjacency_list_array[i].nb_members = 0;
  }

  return graph;
}


/**
 * Destroy the graph
 */
void freeGraph(graph_t *graph) {
  if (graph != NULL) {
    if (graph->adjacency_list_array != NULL) {
      // Free the adjacency list array
      for (int v = 0; v < graph->nb_vertices; v++) {
        adjacency_list_node_t *adjacency_list_item = graph->adjacency_list_array[v].head;
        while (adjacency_list_item != NULL) {
          adjacency_list_node_t *tmp = adjacency_list_item;
          adjacency_list_item = adjacency_list_item->next;
          free(tmp);
        }
      }
    }
    // Free the graph
    free(graph);
  }
}


/**
 * Add an edge to a graph.
 */
void addEdge(graph_t *graph, int src, int dest, int weight) {
  // Create a new adjacency list node
  adjacency_list_node_t *newNode = (adjacency_list_node_t *) malloc(sizeof(adjacency_list_node_t));
  if (newNode == NULL) {
    fprintf(stderr, "Unable to allocate memory for new node in adjacency list.\n");
    exit(EXIT_FAILURE);
  }
  newNode->vertex = dest;
  newNode->weight = weight;

  // Add it in place of the head of the list
  newNode->next = graph->adjacency_list_array[src].head;

  graph->adjacency_list_array[src].head = newNode;
  graph->adjacency_list_array[src].nb_members++;
}


/**
 * Pretty print
 */
void printGraph(graph_t *graph) {
  for (int i = 0; i < graph->nb_vertices; i++) {
    adjacency_list_node_t *adjacency_list_item = graph->adjacency_list_array[i].head;
    printf("\n%d: ", i);
    while (adjacency_list_item != NULL) {
      printf("%d (%d) -> ", adjacency_list_item->vertex, adjacency_list_item->weight);
      adjacency_list_item = adjacency_list_item->next;
    }
    printf("NULL\n");
  }
}


/**
 * Reverse a graph
 */
graph_t *reverseGraph(graph_t *graph) {
  graph_t *reversed_graph = createGraph(graph->nb_vertices);

  for (int i = 0; i < graph->nb_vertices; i++) {
    adjacency_list_node_t *adjacency_list_item = graph->adjacency_list_array[i].head;
    while (adjacency_list_item != NULL) {
      addEdge(reversed_graph, adjacency_list_item->vertex, i, adjacency_list_item->weight);
      adjacency_list_item = adjacency_list_item->next;
    }
  }

  return reversed_graph;
}


/**
 * Do a DFS starting from specified node.
 */
void dfs (graph_t *graph, int *d, int *pi, int *f) {
  int date = 0;
  states_t *states = initStates(graph);  // Set all nodes to not visited

  pi = (int *) malloc(sizeof(int) * graph->nb_vertices);
  if (pi == NULL) {
    fprintf(stderr, "Unable to allocate memory to store parent informations.\n");
    exit(EXIT_FAILURE);
  }
  f = (int *) malloc(sizeof(int) * graph->nb_vertices);
  if (f == NULL) {
    fprintf(stderr, "Unable to allocate memory to store discovery dates.\n");
    exit(EXIT_FAILURE);
  }
  d = (int *) malloc(sizeof(int) * graph->nb_vertices);
  if (f == NULL) {
    fprintf(stderr, "Unable to allocate memory to store ending dates.\n");
    exit(EXIT_FAILURE);
  }

  for (int u = 0; u < graph->nb_vertices; u++) {
    pi[u] = -1;
  }

  for (int u = 0; u < graph->nb_vertices; u++) {
    if (isState(states, u, NOT_VISITED)) {
      dfsVisit(graph, u, states, &date, d, pi, f);
    }
  }

  destroyStates(states);
}


/**
 * Auxiliary DFS function.
 */
void dfsVisit(graph_t *graph, int u, states_t *states, int *date, int *d, int *pi, int *f) {
  markNode(states, u, VISITING);
  (*date)++;

  d[u] = *date;

  for (int v = 0; v < graph->adjacency_list_array[u].nb_members; v++) {
    if (isState(states, v, NOT_VISITED)) {
      pi[v] = u;
      dfsVisit(graph, v, states, date, d, pi, f);
    }
  }

  markNode(states, u, VISITED);
  (*date)++;
  f[u] = *date;
}


/**
 * Check whether the graph is connected or not.
 */
bool isConnected(graph_t *graph) {
  int *d = NULL, *pi = NULL, *f = NULL;
  dfs(graph, d, pi, f);

  graph_t *reversed_graph = reverseGraph(graph);

  dfs(reversed_graph, d, pi, f); // TODO - Treat it in decreasing order of f

  freeGraph(reversed_graph);
  free(pi);
  free(f);

  // TODO : Correct return

  return true;
}
