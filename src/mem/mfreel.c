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
  c4ls_init(&self->dead_its);
  c4ls_init(&self->live_its);
  return self;
}

static void free_its(struct c4mfreel *self, struct c4ls *root) {
  C4LS_DO(root, _it) {
    struct c4mpool_it *it = C4PTROF(c4mpool_it, its_node, _it);
    c4malloc_release(self->src->src, it);
  }
}

void c4mfreel_free(struct c4mfreel *self) {
  free_its(self, &self->dead_its);
  free_its(self, &self->live_its);
  c4malloc_free(&self->malloc);
}

void *c4mfreel_acquire(struct c4mfreel *self, size_t size) {
  C4LS_DO(&self->live_its, _it) {
    struct c4mpool_it *it = C4PTROF(c4mpool_it, its_node, _it);
    bool *skipped = (bool *)it->data;
    
    if (size <= it->size) {
      c4ls_delete(_it);
      return c4mpool_add(self->src, it) + sizeof(bool);
    }

    if (*skipped) {
      c4ls_delete(_it);
      c4ls_prepend(&self->dead_its, _it);
    } else { *skipped = true; }
  }

  return c4mpool_acquire(self->src, size + sizeof(bool)) + sizeof(bool);
}

void c4mfreel_release(struct c4mfreel *self, void *ptr) {
  struct c4mpool_it *it = c4mpool_remove(self->src, ptr - sizeof(bool));
  bool *skipped = (bool *)it->data;
  *skipped = false;
  c4ls_append(&self->live_its, &it->its_node);
}
