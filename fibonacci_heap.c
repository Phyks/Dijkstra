#include <stdio.h>
#include <stdlib.h>

#include "fibonacci_heap.h"

/**
 * Create an empty Fibonacci heap.
 */
fibonacci_heap_t *fibonacciHeapCreate() {
  fibonacci_heap_t *fh = (fibonacci_heap_t *) malloc(sizeof(fibonacci_heap_t));
  if(fh == NULL) {
    fprintf(stderr, "Unable to allocate memory for the Fibonacci heap.\n");
    exit(EXIT_FAILURE);
  }
  fibonacci_heap_element_t *root = (fibonacci_heap_element_t *) malloc(sizeof(fibonacci_heap_element_t));
  if(root == NULL) {
    fprintf(stderr, "Unable to allocate memory for the Fibonacci heap root.\n");
    exit(EXIT_FAILURE);
  }

  root->parent = NULL;
  root->child = NULL;
  root->left = NULL;
  root->right = NULL;
  root->degree = 0;
  root->key = NIL;

  fh->nb_nodes = 0;
  fh->min = NULL;
  fh->root = root;

  return fh;
}


/**
 * Free a Fibonacci heap.
 */
void fibonacciHeapFree(fibonacci_heap_t *fh) {
  // TODO
  free(fh);
}


/**
 * Check whether a Fibonacci heap is empty or not.
 */
int fibonacciHeapIsEmpty(fibonacci_heap_t const *fh) {
  return fh->nb_nodes == 0;
}


/**
 * Find the minimum of the Fibonacci heap.
 */
fibonacci_heap_element_t *fibonacciHeapMin(fibonacci_heap_t const *fh) {
  return fh->min;
}


/**
 * Add a node to the roots of a Fibonacci heap.
 */
void fibonacciHeapAddRoot(fibonacci_heap_t *fh, fibonacci_heap_element_t *node) {
  fibonacci_heap_element_t *root = fh->root;
  while (root->right != NULL) {
    root = root->right;
  }
  root->right = node;
}


/**
 * Delete a node from the roots of a Fibonacci heap.
 */
void fibonacciHeapDeleteRoot(fibonacci_heap_t *fh, fibonacci_heap_element_t *node) {
  if (node == node->right) {
    fh->root = NULL;
  }
  else {
    node->right->left = node->left;
  }
}


/**
 * Insert an element in a Fibonacci heap.
 */
void fibonacciHeapInsert(fibonacci_heap_t *fh, fibonacci_heap_element_t *e) {
  fibonacci_heap_element_t *node = (fibonacci_heap_element_t *) malloc(sizeof(fibonacci_heap_element_t));
  if(node == NULL) {
    fprintf(stderr, "Unable to allocate memory for the new node in the Fibonacci heap.\n");
    exit(EXIT_FAILURE);
  }

  node->degree = 0;
  node->parent = NULL;
  node->child = NULL;
  node->left = NULL;
  node->right = NULL;

  fibonacciHeapAddRoot(fh, node);

  if (fh->min == NULL || node->key < fh->min->key) {
    fh->min = node;
  }
  fh->nb_nodes++;
}


/**
 * Remove an element from a Fibonacci heap.
 */
void fibonacciHeapRemove(fibonacci_heap_t *fh, int x) {
  // TODO
}


/**
 * Extract the minimum from a Fibonacci heap.
 */
static void fibonacciHeapConsolidate(fibonacci_heap_t *fh) {
  // TODO
}
static void fibonacciHeapLinkHeaps(fibonacci_heap_t *fh, fibonacci_heap_element_t *y, fibonacci_heap_element_t *x) {
  fibonacciHeapDeleteRoot(fh, y);
  // TODO
}
fibonacci_heap_element_t fibonacciHeapExtractMin(fibonacci_heap_t *fh) {
  fibonacci_heap_element_t *z = fh->min;
  if (z != NULL) {
    /*for (x child of z) {  // TODO
      fibonacciHeapAddRoot(fh, x);
      x->parent = NULL;
    }*/

    fibonacciHeapDeleteRoot(fh, z);

    if (z == z->right) {
      fh->min = NULL;
    }
    else {
      fh->min = z->right;
      fibonacciHeapConsolidate(fh);
    }

    fh->nb_nodes--;
  }

  return *z;
}
