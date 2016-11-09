#include <stdlib.h>
#include "map.h"

struct c4map *c4map_init(struct c4map *self, c4cmp_t cmp) {
  self->cmp = cmp;
  c4slab_init(&self->its, sizeof(struct c4map_it));
  self->len = 0;
  return self;
}

void c4map_free(struct c4map *self) {
  c4slab_free(&self->its);
}
