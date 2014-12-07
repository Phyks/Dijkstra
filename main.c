#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graph.h"
#include "dijkstra.h"
#include "utils.h"
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
 *
 * @todo Read better the file, to be able to parse large files.
 */
graph_t *parse_input_file (char *file) {
  FILE *fp;
  char line[1024];
  char *token;

  int vertices;
  int i = 0;
  int edge[2];
  double weight;

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

        if (edge[0] >= vertices || edge[1] >= vertices) {
          fprintf(stderr, "Invalid graph provided.\n");
          exit(EXIT_FAILURE);
        }

        token = strtok(NULL, " ");
        weight = atof(token);

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


void pretty_print_float (float f) {
  char s[50];
  char *p;

  sprintf(s, "%f", f);
  for (p=s; *p; ++p) {
    if ('.' == *p) {
      while (*++p);
      while ('0' == *--p) {
        *p = '\0';
      }
      if ('.' == *p) {
        *p = '\0';
      }
    }
  }
  printf("%s", s);
}


int main(int argc, char **argv) {
  if (argc < 2) {
    fprintf(stderr, "No input graph.\n\nUsage:\n\t");
    fprintf(stderr, "%s", argv[0]);
    fprintf(stderr, " graph\n");
    exit(EXIT_FAILURE);
  }

  graph_t *graph = parse_input_file(argv[1]);

  if (!isConnected(graph)) {
    fprintf(stderr, "Input graph is not connected.\n");
    exit(EXIT_FAILURE);
  }

  int start = 0;
  double *dist = (double*) safe_malloc(sizeof(double)*graph->nb_vertices);
  int *prev = (int*) safe_malloc(sizeof(int)*graph->nb_vertices);
  dijkstra(graph, start, prev, dist);

  for (int i=0; i<graph->nb_vertices; i++) {
    printf("%d\t", i);
    if (dist[i] == INFINITY)
      printf("-\t-\n");
    else {
      pretty_print_float(dist[i]);
      printf("\t");
      if (prev[i] == INT_MAX)           /* unreachable */
        printf("-\n");
      else
        printf("%d\n", prev[i]);
    }
  }

  freeGraph(graph);
  free(dist);
  free(prev);
  exit(EXIT_SUCCESS);
}





