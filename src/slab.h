#ifndef C4LIFE_SLAB
#define C4LIFE_SLAB

#include <stddef.h>

struct c4slab {
  size_t len, slot_size;
  void *slots;
};

struct c4slab *c4slab_init(struct c4slab *self, size_t size);
void c4slab_free(struct c4slab *self);

struct c4slab *c4slab_grow(struct c4slab *self, size_t len);

#endif
