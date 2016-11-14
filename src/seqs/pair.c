#include "pair.h"
#include "utils.h"

struct c4pair *c4pair(size_t left, size_t right) {
  struct c4pair *p = c4acquire(sizeof(struct c4pair) + left + right);
  p->offs = left;
  return p;
}

void *c4pair_left(struct c4pair *self) { return self->data; }

void *c4pair_right(struct c4pair *self) { return self->data + self->offs; }
