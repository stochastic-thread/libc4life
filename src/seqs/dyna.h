#ifndef C4DYNA_H
#define C4DYNA_H

#include "slab.h"

#define C4DYNA(var, it_size)			\
  struct c4dyna var;				\
  c4dyna_init(&var, it_size);			\

struct c4dyna {
  size_t len;
  struct c4slab its;
};

struct c4dyna_seq {
  struct c4seq super;
  struct c4dyna *array;
};

struct c4dyna *c4dyna_init(struct c4dyna *self, size_t it_size);
void c4dyna_free(struct c4dyna *self);

void c4dyna_delete(struct c4dyna *self, size_t idx);
void c4dyna_grow(struct c4dyna *self, size_t len);
void *c4dyna_idx(struct c4dyna *self, size_t idx);
void *c4dyna_insert(struct c4dyna *self, size_t idx);
void *c4dyna_pop(struct c4dyna *self);
void *c4dyna_push(struct c4dyna *self);
struct c4seq *c4dyna_seq(struct c4dyna *self, struct c4dyna_seq *seq);

#endif
