#include <stdio.h>
#include <stdlib.h>
#include "graph.h"


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
    if (graph) {
        if (graph->adjacency_list_array) {
            // Free the adjacency list array
            for (int v = 0; v < graph->nb_vertices; v++) {
                adjacency_list_node_t *adjacency_list_item = graph->adjacency_list_array[v].head;
                while (adjacency_list_item) {
                    free(adjacency_list_item->next);
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
        while (adjacency_list_item) {
            printf("%d (%d) -> ", adjacency_list_item->vertex, adjacency_list_item->weight);
            adjacency_list_item = adjacency_list_item->next;
        }
        printf("NULL\n");
    }
}
