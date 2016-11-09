#include <stddef.h>
#include <assert.h>
#include "ctx.h"

struct c4ctx *c4ctx_init(struct c4ctx *self) {
  self->try = NULL;
  return self;
}

void c4ctx_free(struct c4ctx *self) {
  assert(!self->try);
}
