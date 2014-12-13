#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "fibonacci_heap.h"
#include "utils.h"

/**
 * Create an empty Fibonacci heap.
 */
fibonacci_heap_t *fibonacciHeapCreate() {
  fibonacci_heap_t *fh =
    (fibonacci_heap_t *) safe_malloc(sizeof(fibonacci_heap_t));

  fh->nb_nodes = 0;
  fh->min = NULL;
  fh->root = NULL;

  return fh;
}


/**
 * Free a Fibonacci heap.
 * TODO
 */
void fibonacciHeapFree(fibonacci_heap_t *fh) {
  fibonacci_heap_element_t *level = fh->root;
  fibonacci_heap_element_t *parallel, *tmp;
  while (level->child != NULL) {  // Go all the way down
    level = level->child;
  }
  while (level != NULL) {  // Go back to top, freeing the structure
    if (level->left != level) {  // If not a single child
      parallel = level;
      while (parallel != NULL) {  // Go to the left on each level
        tmp = parallel->left;
        free(parallel);
        parallel = tmp;
      }
      parallel = level->right;
      while (parallel != NULL) {  // Then, to the right
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
 * Crate an empty new element ready to be inserted in a fibonacci heap.
 */
fibonacci_heap_element_t *fibonacciHeapNewElement(float key, int vertex) {
  fibonacci_heap_element_t *e = safe_malloc(sizeof(fibonacci_heap_element_t));
  e->key = key;
  e->vertex = vertex;
  e->degree = 0;
  e->child = NULL;
  e->parent = NULL;
  e->left = NULL;
  e->right = NULL;
  return e;
}


/**
 * Add a node to the roots of a Fibonacci heap.
 */
void fibonacciHeapAddRoot(fibonacci_heap_t *fh, fibonacci_heap_element_t *node) {
  fibonacci_heap_element_t *root = fh->root;
  node->parent = NULL;
  if (root == NULL) {  // Empty heap
    node->left = node;
    node->right = node;
    fh->root = node;
  }
  else {
    if (root->right != root) {  // Actual root is not a single element
      while (root->right != NULL) {
        printf("infinite loop\n");
        root = root->right;
      }
    }
    else {
      root->left = NULL;
    }
    root->right = node;
    node->left = root;
    node->right = NULL;
  }
  ++fh->nb_nodes;
}


/**
 * Delete a node from the roots of a Fibonacci heap.
 *
 * User must manually free.
 */
void fibonacciHeapDeleteRoot(fibonacci_heap_t *fh, fibonacci_heap_element_t *node) {
  if (node == node->right) {
    fh->root = NULL;
  }
  else {
    if (node->right != NULL) {
      node->right->left = node->left;
      if (node->right->right == NULL && node->right->left == NULL) {
        node->right->right = node->right;
        node->right->left = node->right;
      }
      fh->root = node->right;
    }
    else {  // node->left is not NULL in this case
      node->left->right = node->right;
      if (node->left->right == NULL && node->left->left == NULL) {
        node->left->right = node->left;
        node->left->left = node->left;
      }
      fh->root = node->left;
    }
  }
  --fh->nb_nodes;
}


/**
 * Insert an element in a Fibonacci heap.
 */
void fibonacciHeapInsert(fibonacci_heap_t *fh, fibonacci_heap_element_t *e) {
  fibonacciHeapAddRoot(fh, e);

  if (fh->min == NULL || e->key < fh->min->key) {
    fh->min = e;
  }
  ++fh->nb_nodes;
}


/**
 * Extract the minimum from a Fibonacci heap.
 */
static void fibonacciHeapAppend(fibonacci_heap_element_t *x, fibonacci_heap_element_t *y) {
  y->left = y;
  y->right = y;
  y->parent = x;
  /* Make y a child of x and increment degree of x */
  if (x->child == NULL) {
    x->child = y;
  }
  else {
    fibonacci_heap_element_t *child = x->child;
    if (child->right != child) {
      while (child->right != NULL) {
        child = child->right;
      }
    }
    else {
      child->left = NULL;
    }
    child->right = y;
    y->left = child;
    y->right = NULL;
  }
  x->degree++;
}
static void fibonacciHeapLinkHeaps(fibonacci_heap_t *fh, fibonacci_heap_element_t *y, fibonacci_heap_element_t *x) {
  fibonacciHeapDeleteRoot(fh, y);
  fibonacciHeapAppend(x, y);
}
static void fibonacciHeapConsolidate(fibonacci_heap_t *fh) {
  const int max_deg = (int) floor(log(fh->nb_nodes) / log((1+sqrt(5)) / 2)) + 1;
  fibonacci_heap_element_t **A = (fibonacci_heap_element_t **) safe_malloc(sizeof(fibonacci_heap_element_t *) * max_deg);
  for (int i = 0; i < max_deg; i++) {
    A[i] = NULL;
  }

  int d;
  fibonacci_heap_element_t *w;
  w = fh->root;
  // Go all the way to the left
  if (w->left != w) {
    while (w->left != NULL) {
      w = w->left;
    }
  }
  // Iterate over all the roots
  while (w != NULL) {
    fibonacci_heap_element_t *x = w;
    d = x->degree;
    while (A[d] != NULL) {
      fibonacci_heap_element_t *y = A[d];
      if (x->key > y->key) {
        fibonacci_heap_element_t *tmp = y;
        y = x;
        x = tmp;
      }
      fibonacciHeapLinkHeaps(fh, y, x);
      A[d] = NULL;
      d++;
    }
    A[d] = x;
    if (w->right != w) {
      w = w->right;
    }
    else {
      w = NULL;
    }
  }
  fh->min = NULL;
  for (int i = 0; i < max_deg; i++) {
    if (A[i] != NULL) {
      fibonacciHeapAddRoot(fh, A[i]);
      if (fh->min == NULL || A[i]->key < fh->min->key) {
        fh->min = A[i];
      }
    }
  }
}
fibonacci_heap_element_t *fibonacciHeapExtractMin(fibonacci_heap_t *fh) {
  fibonacci_heap_element_t *z = fh->min;
  if (z != NULL) {
    if (z->child != NULL) {  // Put all the childs of z at the root level
      fibonacci_heap_element_t *x = z->child;
      if (x->left != x) {
        fibonacci_heap_element_t *tmp = x;
        while (tmp != NULL) {  // First, go left
          fibonacciHeapAddRoot(fh, tmp);
          tmp = tmp->left;
        }
        tmp = x->right;
        while (tmp != NULL) {  // Then, go right
          fibonacciHeapAddRoot(fh, tmp);
          tmp = tmp->right;
        }
      }
      else {
        fibonacciHeapAddRoot(fh, x);
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

  return z;
}
