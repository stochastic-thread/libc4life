#ifndef C4MFREEL_H
#define C4MFREEL_H

#include "malloc.h"
#include "seqs/ls.h"

#define C4MFREEL(var, src)				\
  struct c4mfreel var;					\
  c4mfreel_init(&var, src);				\

struct c4mpool;

struct c4mfreel {
  struct c4ls dead_its, live_its;
  struct c4malloc malloc;
  struct c4mpool *src;
};

struct c4mfreel *c4mfreel_init(struct c4mfreel *self, struct c4mpool *src);
void c4mfreel_free(struct c4mfreel *self);

void *c4mfreel_acquire(struct c4mfreel *self, size_t size);
void c4mfreel_release(struct c4mfreel *self, void *ptr);
void *c4mfreel_require(struct c4mfreel *self, void *ptr, size_t size);

#endif
