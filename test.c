#include <stdio.h>
#include <stdlib.h>
#include "fibonacci_heap.h"

int main() {
  fibonacci_heap_t *fb = fibonacciHeapCreate();

  printf("%d\n", fibonacciHeapIsEmpty(fb)); // 1
  printf("%d\n", fb->nb_nodes); // 0

  fibonacciHeapInsert(fb, fibonacciHeapNewElement(1, 1, 1));

  printf("%d\n", fibonacciHeapIsEmpty(fb)); // 0
  printf("%d\n", fb->nb_nodes); // 1

  fibonacciHeapInsert(fb, fibonacciHeapNewElement(2, 2, 2));

  printf("%d\n", fb->nb_nodes); // 2

  fibonacci_heap_element_t *e = fibonacciHeapMin(fb);

  printf("%f\n", e->key); // 1.00000
  printf("%d\n", fb->nb_nodes); // 2

  e = fibonacciHeapExtractMin(fb);
  printf("%f\n", e->key); // 1.000000
  printf("%d\n", fb->nb_nodes); // 1
  free(e);

  printf("%d\n", fibonacciHeapIsEmpty(fb)); // 0

  e = fibonacciHeapExtractMin(fb);
  printf("%f\n", e->key); // 2.00000
  printf("%d\n", fb->nb_nodes); // 0
  free(e);

  printf("%d\n", fibonacciHeapIsEmpty(fb)); // 1

  fibonacciHeapInsert(fb, fibonacciHeapNewElement(1, 1, 1));

  printf("%d\n", fb->nb_nodes); // 1

  fibonacciHeapFree(fb);
  return 0;
}

/*
 * 1
 * 0
 * 0
 * 1
 * 2
 * 1.000000
 * 2
 * 1.000000
 * 1
 * 0
 * 2.000000
 * 0
 * 1
 * 1
 */
