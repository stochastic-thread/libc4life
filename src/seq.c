#include <assert.h>
#include <stddef.h>
#include "seq.h"

struct c4seq *c4seq_init(struct c4seq *self) {
  self->free = NULL;
  self->next = NULL;
  return self;
}

void *c4seq_next(struct c4seq *self) {
  assert(self->next);
  void *it = self->next(self);
  if (!it) { c4seq_free(self); }
  return it;
}

void c4seq_free(struct c4seq *self) {
  if (self->free) { self->free(self); }
}
