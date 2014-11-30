#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "graph.h"


/**
 * Trim/Strip function
 */
char *strstrip(char *s) {
    size_t size;
    char *end;

    size = strlen(s);

    if (!size) {
        return s;
    }

    end = s + size - 1;
    while (end >= s && isspace(*end)) {
        end--;
    }
    *(end + 1) = '\0';

    while (*s && isspace(*s)) {
        s++;
    }

    return s;
}


/**
 * Create a graph structure from an input file.
 */
graph_t *parse_input_file (char *file) {
    FILE *fp;
    char line[1024];
    char *token;

    int vertices;
    int i = 0;
    int edge[2], weight;

    graph_t *graph;

    fp = fopen(file, "r");
    if (fp != NULL) {
        while (fgets(line, 1024, fp) != NULL) {
            if (i == 0) {
                token = strtok(strstrip(line), " ");
                vertices = atoi(token);

                graph = createGraph(vertices);
            }
            else {
                token = strtok(strstrip(line), " ");
                edge[0] = atoi(token);

                token = strtok(NULL, " ");
                edge[1] = atoi(token);

                token = strtok(NULL, " ");
                weight = atoi(token);

                addEdge(graph, edge[0], edge[1], weight);
            }
            i++;
        }
        fclose(fp);
    }
    else {
        fprintf(stderr, "Unable to open input file.\n");
        exit(EXIT_FAILURE);
    }

    return graph;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "No input graph.\n\nUsage:\n\t");
        fprintf(stderr, "%s\n", argv[0]);
        fprintf(stderr, " graph\n");
        exit(EXIT_FAILURE);
    }

    graph_t *graph = parse_input_file(argv[1]);

    printGraph(graph);

    freeGraph(graph);

    exit(EXIT_SUCCESS);
}
