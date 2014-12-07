#ifndef _UTILS_H_
#define _UTILS_H_

typedef struct associative_array {
  int key;
  int value;
} associative_array_t;


void* safe_malloc(int size);
void* safe_realloc(void* prev, int size);
#endif
