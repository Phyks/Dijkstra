cdef extern from "graph.h":
    ctypedef struct adjacency_list_node_t:
        int vertex
        double weight
        adjacency_list_node_t* next
        
    ctypedef struct adjacency_list_t:
        int nb_members
        adjacency_list_node_t* head

    ctypedef struct graph_t:
        int nb_vertices
        adjacency_list_t* adjacency_list_array

    graph_t *createGraph(int n);
    void freeGraph(graph_t* graph);
    void printGraph(graph_t* graph);
    void addEdge(graph_t *graph, int src, int dest, double weight)

cdef extern from "dijkstra.h":
    void dijkstra(graph_t* G, int s, int* prev, double* dist)

cdef extern from "limits.h":
    int INT_MAX
