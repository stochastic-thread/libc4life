#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "macros.h"
#include "mslab.h"

static void *acquire(struct c4malloc *self, size_t size) {
  return c4mslab_acquire(C4PTROF(c4mslab, malloc, self), size);
}

struct c4mslab *c4mslab_init(struct c4mslab *self, size_t it_size) {
  self->it_size = it_size;
  c4malloc_init(&self->malloc);
  self->malloc.acquire = acquire;
  c4ls_init(&self->full_its);
  c4ls_init(&self->live_its);
  return self;
}

void c4mslab_free(struct c4mslab *self) {
  C4LS_DO(&self->full_its, _it) { free(C4PTROF(c4mslab_it, its_node, _it)); }
  C4LS_DO(&self->live_its, _it) { free(C4PTROF(c4mslab_it, its_node, _it)); }
}

void *c4mslab_acquire(struct c4mslab *self, size_t size) {
  assert(size < self->it_size);
  
  C4LS_DO(&self->live_its, _it) {
    struct c4mslab_it *it = C4PTROF(c4mslab_it, its_node, _it);

    if (it->offs == self->it_size) {
	c4ls_delete(&it->its_node);
	c4ls_prepend(&self->full_its, &it->its_node);
    } else if (self->it_size - it->offs >= size) {      
      void *ptr = it->data + it->offs;
      it->offs += size;   
      return ptr;
    } 
  }

  struct c4mslab_it *it = malloc(sizeof(struct c4mslab_it) + self->it_size);
  it->offs = size;
  c4ls_prepend(&self->live_its, &it->its_node);
  return it->data;
}

struct c4mslab_it *c4mslab_it(struct c4mslab *self) {
  return c4ls_empty(&self->live_its)
    ? NULL
    : C4PTROF(c4mslab_it, its_node, self->live_its.prev);
}
