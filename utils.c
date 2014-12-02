#include <stdlib.h>
#include <stdio.h>

void* safe_malloc(int size) {
  void* ret = malloc(size);
  if (ret == NULL) {
    fprintf(stderr, "Unable to allocate memory.\n");
    exit(EXIT_FAILURE);
  }
  return ret;
}

void* safe_realloc(void* prev, int size) {
  void* ret = realloc(prev, size);
  if (ret == NULL) {
    fprintf(stderr, "Unable to reallocate memory.\n");
    exit(EXIT_FAILURE);
  }
  return ret;
}


