#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "mfreel.h"
#include "mpool.h"
#include "macros.h"

static void *acquire(struct c4malloc *_self, size_t size) {
  struct c4mfreel *self = C4PTROF(c4mfreel, malloc, _self);
  return c4mfreel_acquire(self, size);
}

static void release(struct c4malloc *_self, void *ptr) {
  struct c4mfreel *self = C4PTROF(c4mfreel, malloc, _self);
  c4mfreel_release(self, ptr);
}

struct c4mfreel *c4mfreel_init(struct c4mfreel *self, struct c4mpool *src) {
  self->src = src;
  c4malloc_init(&self->malloc);
  self->malloc.acquire = acquire;
  self->malloc.release = release;
  c4ls_init(&self->its);
  return self;
}

void c4mfreel_free(struct c4mfreel *self) {
  C4LS_DO(&self->its, _it) {
    struct c4mpool_it *it = C4PTROF(c4mpool_it, its_node, _it);
    c4malloc_release(self->src->src, it);
  }
}

void *c4mfreel_acquire(struct c4mfreel *self, size_t size) {
  C4LS_DO(&self->its, _it) {
    struct c4mpool_it *it = C4PTROF(c4mpool_it, its_node, _it);
    if (it->size >= size) {
      c4ls_delete(_it);
      return c4mpool_add(self->src, it);
    }
  }

  return c4malloc_acquire(self->src->src, size);
}

void c4mfreel_release(struct c4mfreel *self, void *ptr) {
    struct c4mpool_it *it = c4mpool_remove(self->src, ptr);
    c4ls_prepend(&self->its, &it->its_node);
}
