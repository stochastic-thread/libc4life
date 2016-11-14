#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "macros.h"
#include "mslab.h"

static void *acquire(struct c4malloc *self, size_t size) {
  return c4mslab_acquire(C4PTROF(c4mslab, malloc, self), size);
}

struct c4mslab *c4mslab_init(struct c4mslab *self, size_t it_size,
			     struct c4malloc *src) {
  self->it_size = it_size;
  self->src = src;
  c4malloc_init(&self->malloc);
  self->malloc.acquire = acquire;
  c4ls_init(&self->dead_its);
  c4ls_init(&self->live_its);
  return self;
}

static void free_its(struct c4mslab *self, struct c4ls *root) {
  C4LS_DO(root, _it) {
    c4malloc_release(self->src, C4PTROF(c4mslab_it, its_node, _it));
  }
}

void c4mslab_free(struct c4mslab *self) {
  free_its(self, &self->dead_its);
  free_its(self, &self->live_its);
}

void *c4mslab_acquire(struct c4mslab *self, size_t size) {
  C4LS_DO(&self->live_its, _it) {
    struct c4mslab_it *it = C4PTROF(c4mslab_it, its_node, _it);

    if (size <= it->size - it->offs) {      
      void *ptr = it->data + it->offs;
      it->offs += size;   
      return ptr;
    } else if (it->skipped || it->offs == it->size) {
	c4ls_delete(&it->its_node);
	c4ls_prepend(&self->dead_its, &it->its_node);
    }

    it->skipped = true;
  }

  bool full = size > self->it_size;
  size_t it_size = full ? size : self->it_size;
  
  struct c4mslab_it *it =
    c4malloc_acquire(self->src, sizeof(struct c4mslab_it) + it_size);
  it->skipped = false;
  it->size = it_size;
  it->offs = size;
  c4ls_prepend(full ? &self->dead_its : &self->live_its, &it->its_node);
  return it->data;
}

struct c4mslab_it *c4mslab_it(struct c4mslab *self) {
  return c4ls_empty(&self->live_its)
    ? NULL
    : C4PTROF(c4mslab_it, its_node, self->live_its.prev);
}
