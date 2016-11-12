#ifndef C4MPOOL_H
#define C4MPOOL_H

#include <stddef.h>
#include "seqs/ls.h"

#define C4MPOOL(var)				\
  struct c4mpool var;				\
  c4mpool_init(&var);				\

#define C4MPOOL_ALLOC(mpool, type, cnt)		\
  c4mpool_alloc(mpool, sizeof(type) * cnt)	\

typedef struct c4ls c4mpool_prefix_t;

struct c4mpool {
  struct c4ls ptrs;
};

struct c4mpool *c4mpool_init(struct c4mpool *self);
void c4mpool_free(struct c4mpool *self);

void *c4mpool_add(struct c4mpool *self, struct c4ls *ptr);
void *c4mpool_alloc(struct c4mpool *self, size_t size);
struct c4ls *c4mpool_remove(struct c4mpool *self, void *ptr);

#endif
