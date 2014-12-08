#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "fibonacci_heap.h"
#include "utils.h"

/**
 * Create an empty Fibonacci heap.
 */
fibonacci_heap_t *fibonacciHeapCreate() {
  fibonacci_heap_t *fh = (fibonacci_heap_t *) safe_malloc(sizeof(fibonacci_heap_t));
  fibonacci_heap_element_t *root = (fibonacci_heap_element_t *) safe_malloc(sizeof(fibonacci_heap_element_t));

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
  fibonacci_heap_element_t *level = fh->root;
  fibonacci_heap_element_t *parallel, *tmp;
  while (level->child != NULL) {  // Go all the way down
    level = level->child;
  }
  while (level->parent != NULL) {  // Go back to top, freeing the structure
    parallel = level;
    while (parallel->left != NULL) {  // Go to the left on each level
      tmp = parallel->left;
      free(parallel);
      parallel = tmp;
    }
    parallel = level->right;
    if (parallel != NULL) {
      while (parallel->right != NULL) {  // Then, to the right
        tmp = parallel->right;
        free(parallel);
        parallel = tmp;
      }
    }
    tmp = level->parent;
    free(level);
    level = tmp;
  }
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


/*
 */
fibonacci_heap_element_t *fibonacciHeapNewElement(int key) {
  fibonacci_heap_element_t *e = safe_malloc(sizeof(fibonacci_heap_element_t));
  e->key = key;
  return e;
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
  if (node == node->right || node->right == NULL) {
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
  e->degree = 0;
  e->parent = NULL;
  e->child = NULL;
  e->left = NULL;
  e->right = NULL;

  fibonacciHeapAddRoot(fh, e);

  if (fh->min == NULL || e->key < fh->min->key) {
    fh->min = e;
  }
  fh->nb_nodes++;
}


/**
 * Extract the minimum from a Fibonacci heap.
 */
static void fibonacciHeapAppend(fibonacci_heap_element_t *x, fibonacci_heap_element_t *y) {
  /* Make y a child of x and increment degree of x */
  if (x->child == NULL) {
    x->child = y;
  }
  else {
    fibonacci_heap_element_t *child = x->child;
    while (child->right != NULL) {
      child = child->right;
    }
    child->right = y;
  }
  x->degree++;
}
static void fibonacciHeapLinkHeaps(fibonacci_heap_t *fh, fibonacci_heap_element_t *y, fibonacci_heap_element_t *x) {
  fibonacciHeapDeleteRoot(fh, y);
  fibonacciHeapAppend(x, y);
}
static void fibonacciHeapConsolidate(fibonacci_heap_t *fh) {
  fibonacci_heap_element_t **A = (fibonacci_heap_element_t **) safe_malloc(sizeof(fibonacci_heap_element_t *) * floor(log(fh->nb_nodes) / log((1+sqrt(5)) / 2)));
  for (int i = 0; i < floor(log(fh->nb_nodes) / log((1+sqrt(5)) / 2)); i++) {
    A[i] = NULL;
  }

  int d;
  fibonacci_heap_element_t *w;
  w = fh->root;
  // Go all the way to the left
  while (w->left != NULL) {
    w = w->left;
  }
  while (w->right != NULL) {
    fibonacci_heap_element_t *x = w;
    d = x->degree;
    while (A[d] != NULL) {
      if (x->key > A[d]->key) {
        fibonacci_heap_element_t *tmp = A[d];
        A[d] = x;
        x = tmp;
      }
      fibonacciHeapLinkHeaps(fh, A[d], x);
      A[d] = NULL;
      d++;
    }
    w = w->right;
  }
  fh->min = NULL;
  for (int i = 0; i < floor(log(fh->nb_nodes) / log((1+sqrt(5)) / 2)); i++) {
    if (A[i] != NULL) {
      fibonacciHeapAddRoot(fh, A[i]);
      if (fh->min == NULL || A[i]->key < fh->min->key) {
        fh->min = A[i];
      }
    }
  }
}
fibonacci_heap_element_t fibonacciHeapExtractMin(fibonacci_heap_t *fh) {
  fibonacci_heap_element_t *z = fh->min;
  if (z != NULL) {
    fibonacci_heap_element_t *x = z->child;
    fibonacci_heap_element_t *tmp = x;
    while (tmp->left != NULL) {  // First, go left
      fibonacciHeapAddRoot(fh, tmp);
      tmp->parent = NULL;
      tmp = tmp->left;
    }
    tmp = x->right;
    if (tmp != NULL) {  // Then, go right
      while (tmp->right != NULL) {
        fibonacciHeapAddRoot(fh, tmp);
        tmp->parent = NULL;
        tmp = tmp->right;
      }
    }

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
