from libc.stdlib cimport malloc, free
from cpython cimport array
from pyjkstra cimport dijkstra as c_dijkstra
from pyjkstra cimport graph_t, createGraph, freeGraph, printGraph, addEdge, INT_MAX

cdef class c_Graph:
    '''Cython class that implements a graph'''
    cdef graph_t * thisptr

    def __cinit__(self, int n):
        self.thisptr = createGraph(n)
        if self.thisptr is NULL:
            raise MemoryError

    def __dealloc__(self):
        if self.thisptr is not NULL:
            freeGraph(self.thisptr)
        
    def __str__(self):
        # Bad hack, it prints but returns an empty string …
        printGraph(self.thisptr)
        return ""


    @property
    def nb_vertices(self):
        return self.thisptr.nb_vertices

    def addEdge(self, int src, int dest, double weight):
        addEdge(self.thisptr, src, dest, weight)

    def dijkstra (self, int s):
        # Convert the python object into the c_graph cdef class
        # cdef c_Graph graph = <c_Graph>self.c_graph
        cdef int l = self.thisptr.nb_vertices

        cdef int* prev_arg = <int*>malloc(sizeof(int)*l)
        cdef double* dist_arg = <double*>malloc(sizeof(double)*l)

        c_dijkstra(self.thisptr, s, prev_arg, dist_arg)

        prev_out = []
        dist_out = []
        # Convert back from C-types to python object
        for i in range(l):
            if (prev_arg[i] == INT_MAX):
                val = float("inf")
            else:
                val = prev_arg[i]
                
            prev_out.append(val)

            
            if (dist_arg[i] == INT_MAX):
                val = float("inf")
            else:
                val = dist_arg[i]
                
            dist_out.append(val)

        free(dist_arg)
        free(prev_arg)
        return (prev_out, dist_out)

class Graph:
    ''' A graph represented as an adjacency list.'''
    c_graph = None
    def __init__(self, int n):
        self.c_graph = c_Graph(n)
        
    def __del__(self):
        del self.c_graph
        
    def __str__(self):
        return self.c_graph.__str__()
    
    @property
    def nb_vertices(self):
        return self.c_graph.nb_vertices

    def addEdge(self, int src, int dest, double weight):
        self.c_graph.addEdge(src, dest, weight)


    def dijkstra (self, int s):
        ''' Implement the dijkstra path-finding algorithm.
        Parameters:
        G    dijkstra.Graph    a python representation of a graph
        s    int               the starting point of the algorithm
    
        Returns:
        (prev, dist) with prev the antecedent in the path and dist the distance of
        each node from the start
        '''
        return self.c_graph.dijkstra(s)
