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

struct c4mfreel *c4mfreel_init(struct c4mfreel *self, size_t it_size,
			       struct c4mpool *src) {
  self->it_size = it_size;
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
  assert(size < self->it_size);
  if (c4ls_empty(&self->its)) { return c4malloc_acquire(self->src->src, size); }
  struct c4ls *_it = self->its.next;
  c4ls_delete(_it);
  struct c4mpool_it *it = C4PTROF(c4mpool_it, its_node, _it);
  return c4mpool_add(self->src, it);
}

void c4mfreel_release(struct c4mfreel *self, void *ptr) {
    struct c4mpool_it *it = c4mpool_remove(self->src, ptr);
    c4ls_prepend(&self->its, &it->its_node);
}
