#include <stdlib.h>
#include "mpool.h"

static void *acquire(struct c4malloc *_self, size_t size) {
  return c4mpool_acquire(C4PTROF(c4mpool, malloc, _self), size);
}

static void release(struct c4malloc *_self, void *ptr) {
  c4mpool_release(C4PTROF(c4mpool, malloc, _self), ptr); 
}

static void *require(struct c4malloc *_self, void *ptr, size_t size) {
  return c4mpool_require(C4PTROF(c4mpool, malloc, _self), ptr, size);
}

struct c4mpool *c4mpool_init(struct c4mpool *self, struct c4malloc *src) {
  self->src = src;
  c4ls_init(&self->its);
  c4malloc_init(&self->malloc);
  self->malloc.acquire = acquire;
  self->malloc.release = release;
  self->malloc.require = require;
  return self;
}

void c4mpool_free(struct c4mpool *self) {
  C4LS_DO(&self->its, _it) { free(C4PTROF(c4mpool_it, its_node, _it)); }
}

void *c4mpool_add(struct c4mpool *self, struct c4mpool_it *it) {
  c4ls_prepend(&self->its, &it->its_node);
  return it->ptr;
}

void *c4mpool_acquire(struct c4mpool *self, size_t size) {
  struct c4mpool_it *it = malloc(sizeof(struct c4mpool_it) + size);
  it->size = size;
  return c4mpool_add(self, it);
}

void *c4mpool_require(struct c4mpool *self, void *ptr, size_t size) {
  struct c4mpool_it *it = C4PTROF(c4mpool_it, ptr, ptr);
  c4ls_delete(&it->its_node);
  it = realloc(it, sizeof(struct c4mpool_it) + size);
  it->size = size;
  c4ls_prepend(&self->its, &it->its_node);
  return it->ptr;
}

void c4mpool_release(struct c4mpool *self, void *ptr) {
  free(c4mpool_remove(self, ptr));
}

struct c4mpool_it *c4mpool_remove(struct c4mpool *self, void *ptr) {
  struct c4mpool_it *it = C4PTROF(c4mpool_it, ptr, ptr);
  c4ls_delete(&it->its_node);
  return it;
}
