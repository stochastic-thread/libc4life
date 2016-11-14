#include <stddef.h>
#include "macros.h"
#include "malloc_example.h"

static void *acquire(struct c4malloc *self, size_t size) {
  //struct c4my_malloc *m = C4PTROF(c4my_malloc, malloc, self);

  // Implement your algorithm here
  
  return NULL;
}

static void release(struct c4malloc *self, void *ptr) {
  //struct c4my_malloc *m = C4PTROF(c4my_malloc, malloc, self);

  // Implementing release is optional,
  // as long as all memory is deallocated with the allocator
}

struct c4my_malloc *c4my_malloc_init(struct c4my_malloc *self) {
  c4malloc_init(&self->malloc);
  self->malloc.acquire = acquire;
  self->malloc.release = release;

  // Initialize your state here
  
  return self;
}

void c4my_malloc_free(struct c4my_malloc *self) {
  c4malloc_free(&self->malloc);

  // Free any memory allocated from the heap here
}
