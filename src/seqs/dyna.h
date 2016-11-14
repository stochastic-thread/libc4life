#ifndef C4DYNA_H
#define C4DYNA_H

#include "slab.h"

#define C4DYNA(var, it_size)			\
  struct c4dyna var;				\
  c4dyna_init(&var, it_size);			\

#define _C4DYNA_DO(dyna, it, _idx)				\
  size_t _idx = 0;						\
  for (void *it;						\
       _idx < dyna->len && (it = c4dyna_idx(dyna, _idx));	\
       _idx++)							\

#define C4DYNA_DO(dyna, it)			\
  _C4DYNA_DO((dyna), it, C4GSYM(idx))	\

struct c4dyna {
  size_t len;
  struct c4slab its;
};

struct c4dyna_seq {
  struct c4seq seq;
  struct c4dyna *dyna;
};

struct c4dyna *c4dyna_init(struct c4dyna *self, size_t it_size);
void c4dyna_free(struct c4dyna *self);

void c4dyna_clear(struct c4dyna *self);
void c4dyna_delete(struct c4dyna *self, size_t idx);
void c4dyna_grow(struct c4dyna *self, size_t len);
void *c4dyna_idx(struct c4dyna *self, size_t idx);
void *c4dyna_insert(struct c4dyna *self, size_t idx);
void *c4dyna_pop(struct c4dyna *self);
void *c4dyna_push(struct c4dyna *self);
struct c4seq *c4dyna_seq(struct c4dyna *self, struct c4dyna_seq *seq);

#endif
