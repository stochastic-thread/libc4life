#include <stdlib.h>
#include "mpool.h"

struct c4mpool *c4mpool_init(struct c4mpool *self) {
  c4ls_init(&self->ptrs);
  return self;
}

void c4mpool_free(struct c4mpool *self) {
  C4LS_DO(&self->ptrs, ptr) { free(ptr); }
}

void *c4mpool_add(struct c4mpool *self, struct c4ls *ptr) {
  c4ls_prepend(&self->ptrs, ptr);
  return (void *)ptr + sizeof(struct c4ls);
}

void *c4mpool_alloc(struct c4mpool *self, size_t size) {
  void *ptr = malloc(sizeof(struct c4ls) + size);
  return c4mpool_add(self, ptr);
}

struct c4ls *c4mpool_remove(struct c4mpool *self, void *ptr) {
  struct c4ls *ls = ptr - sizeof(struct c4ls);
  c4ls_delete(ls);
  return ls;
}
