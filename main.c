#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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

void *input (char *file) {
    FILE *fp;
    char line[1024];
    char *token;

    int vertices, edges;
    int i = 0;
    int edge[2], weight;

    fp = fopen(file, "r");
    if (fp != NULL) {
        while (fgets(line, 1024, fp) != NULL) {
            if (i == 0) {
                token = strtok(strstrip(line), " ");
                vertices = atoi(token);

                token = strtok(NULL, " ");
                edges = atoi(token);
            }
            else {
                token = strtok(strstrip(line), " ");
                edge[0] = atoi(token);

                token = strtok(NULL, " ");
                edge[1] = atoi(token);

                token = strtok(NULL, " ");
                weight = atoi(token);
            }
            i++;
        }
        fclose(fp);
    }
    else {
        return NULL;
    }

    printf("%d\n", vertices);
    printf("%d\n\n", edges);
    printf("%d\n", edge[0]);
    printf("%d\n", edge[1]);
    printf("%d\n", weight);

    return NULL;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "No input graph.\n\nUsage:\n\t");
        fprintf(stderr, argv[0]);
        fprintf(stderr, " graph\n");
        exit(EXIT_FAILURE);
    }

    input(argv[1]);

    exit(EXIT_SUCCESS);
}
