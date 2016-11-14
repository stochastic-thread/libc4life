#ifndef C4MALLOC_EXAMPLE_H
#define C4MALLOC_EXAMPLE_H

// Include the c4malloc interface

#include "malloc.h"

// A convenience macro to declare and initialize in one call

#define C4MY_MALLOC(var)				\
  struct c4my_malloc var;				\
  c4my_malloc_init(&var);				\

// The struct containing the allocator state.
// All implementations are required to implement the c4malloc interface.

struct c4my_malloc {
  struct c4malloc malloc;

  // Add your allocator state here
};

// Initializes the internal state of the allocator
struct c4my_alloc *c4my_alloc_init(struct c4my_alloc *self);

// Frees the internal state of the allocator
void c4my_alloc_free(struct c4my_alloc *self);

#endif
