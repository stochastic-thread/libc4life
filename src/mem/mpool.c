#include <stdlib.h>
#include "mpool.h"

struct c4mpool *c4mpool_init(struct c4mpool *self) {
  c4ls_init(&self->its);
  return self;
}

void c4mpool_free(struct c4mpool *self) {
  C4LS_DO(&self->its, _it) { free(C4PTROF(c4mpool_it, its_node, _it)); }
}

void *c4mpool_add(struct c4mpool *self, struct c4mpool_it *it) {
  c4ls_prepend(&self->its, &it->its_node);
  return it->ptr;
}

void *c4mpool_alloc(struct c4mpool *self, size_t size) {
  struct c4mpool_it *it = malloc(sizeof(struct c4mpool_it) + size);
  return c4mpool_add(self, it);
}

void *c4mpool_realloc(struct c4mpool *self, void *ptr, size_t size) {
  struct c4mpool_it *it = C4PTROF(c4mpool_it, ptr, ptr);
  c4ls_delete(&it->its_node);
  it = realloc(it, sizeof(struct c4mpool_it) + size);
  c4ls_prepend(&self->its, &it->its_node);
  return it->ptr;
}

struct c4mpool_it *c4mpool_remove(struct c4mpool *self, void *ptr) {
  struct c4mpool_it *it = C4PTROF(c4mpool_it, ptr, ptr);
  c4ls_delete(&it->its_node);
  return it;
}
