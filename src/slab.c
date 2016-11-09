#include <stdlib.h>
#include "slab.h"

struct c4slab *c4slab_init(struct c4slab *self, size_t slot_size) {
  self->slot_size = slot_size;
  self->slots = NULL;
  return self;
}

void c4slab_free(struct c4slab *self) {
  if (self->slots) { free(self->slots); }
}

struct c4slab *c4slab_grow(struct c4slab *self, size_t len) {
  if (self->len == 0) { self->len = len; }
  else { while (self->len < len) { self->len *= 2; } }
  self->slots = realloc(self->slots, self->len * self->slot_size);
  return self;
}

