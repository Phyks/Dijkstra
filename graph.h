#ifndef _GRAPH_H_
#define _GRAPH_H_

typedef struct adjacency_list_node {
    int vertex;  // Other end of the edge
    int weight;  // Weight of the edge
    struct adjacency_list_node *next;  // Pointer to next item in the adjacency linked list
} adjacency_list_node_t;

typedef struct adjacency_list {
    int nb_members;  // Number of members in the adjacency list
    adjacency_list_node_t *head;  // Head of the adjacency linked list
} adjacency_list_t;

typedef struct graph {
    int nb_vertices;  // Number of vertices
    adjacency_list_t *adjacency_list_array;  // An array of adjacency lists
} graph_t;


graph_t *createGraph(int n);
void freeGraph(graph_t *graph);
void addEdge(graph_t *graph, int src, int dest, int weight);
void printGraph(graph_t *graph);

#endif  // _GRAPH_H_
