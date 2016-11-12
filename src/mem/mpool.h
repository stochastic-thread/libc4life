#ifndef C4MPOOL_H
#define C4MPOOL_H

#include <stddef.h>
#include "malloc.h"
#include "seqs/ls.h"

#define C4MPOOL(var, src)			\
  struct c4mpool var;				\
  c4mpool_init(&var, src);			\

struct c4mpool_it {
  struct c4ls its_node;
  char ptr[];
};

struct c4mpool {
  struct c4ls its;
  struct c4malloc malloc, *src;
};

struct c4mpool *c4mpool_init(struct c4mpool *self, struct c4malloc *src);
void c4mpool_free(struct c4mpool *self);

void *c4mpool_add(struct c4mpool *self, struct c4mpool_it *it);
void *c4mpool_acquire(struct c4mpool *self, size_t size);
void *c4mpool_require(struct c4mpool *self, void *ptr, size_t size);
void c4mpool_release(struct c4mpool *self, void *ptr);
struct c4mpool_it *c4mpool_remove(struct c4mpool *self, void *ptr);

#endif