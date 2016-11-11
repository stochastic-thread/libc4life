#ifndef C4LIFE_SLAB
#define C4LIFE_SLAB

#include <stddef.h>
#include "seq.h"

struct c4slab {
  size_t len, slot_size;
  void *slots;
};

struct c4slab_seq {
  struct c4seq super;
  int line;
  struct c4slab *slab;
};

struct c4slab *c4slab_init(struct c4slab *self, size_t size);
void c4slab_free(struct c4slab *self);

struct c4slab *c4slab_grow(struct c4slab *self, size_t len);
void *c4slab_idx(struct c4slab *self, size_t idx);
void *c4slab_insert(struct c4slab *self, size_t idx);
struct c4seq *c4slab_seq(struct c4slab *self, struct c4slab_seq *seq);
size_t c4slab_size(struct c4slab *self);

#endif
