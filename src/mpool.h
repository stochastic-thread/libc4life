#ifndef C4MPOOL_H
#define C4MPOOL_H

#include <stddef.h>
#include "seqs/ls.h"

#define C4MPOOL(var)				\
  struct c4mpool var;				\
  c4mpool_init(&var);				\

struct c4mpool_it {
  struct c4ls its_node;
  char ptr[];
};

struct c4mpool { struct c4ls its; };

struct c4mpool *c4mpool_init(struct c4mpool *self);
void c4mpool_free(struct c4mpool *self);

void *c4mpool_add(struct c4mpool *self, struct c4mpool_it *it);
void *c4mpool_alloc(struct c4mpool *self, size_t size);
struct c4mpool_it *c4mpool_remove(struct c4mpool *self, void *ptr);

#endif
