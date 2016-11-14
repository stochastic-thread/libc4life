#ifndef MY_MALLOC_H
#define MY_MALLOC_H

// Include the c4malloc interface

#include "malloc.h"

// A convenience macro to declare and initialize in one call

#define MY_MALLOC(var)				\
  struct my_malloc var;				\
  my_malloc_init(&var);				\

// The struct containing the allocator state.
// All implementations are required to implement the c4malloc interface.

struct my_malloc {
  struct c4malloc malloc;

  // Add your allocator state here
};

// Initializes the internal state of the allocator
struct my_alloc *my_alloc_init(struct my_malloc *self);

// Frees the internal state of the allocator
void my_alloc_free(struct my_malloc *self);

#endif
