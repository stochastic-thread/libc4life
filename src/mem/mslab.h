#ifndef C4MSLAB_H
#define C4MSLAB_H

#include "malloc.h"
#include "seqs/ls.h"

#define C4MSLAB(var, it_size)				\
  struct c4mslab var;					\
  c4mslab_init(&var, it_size);				\

struct c4mslab_it {
  size_t offs;
  struct c4ls its_node;
  char data[];
};

struct c4mslab {
  size_t it_size;
  struct c4ls full_its, live_its;
  struct c4malloc malloc;
};

struct c4mslab *c4mslab_init(struct c4mslab *self, size_t it_size);
void c4mslab_free(struct c4mslab *self);

void *c4mslab_acquire(struct c4mslab *self, size_t size);
struct c4mslab_it *c4mslab_it(struct c4mslab *self);

#endif
