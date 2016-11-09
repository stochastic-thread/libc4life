#include <assert.h>
#include "ctx.h"

struct c4ctx *c4ctx_init(struct c4ctx *self) {
  c4ls_init(&self->tries);
  return self;
}

void c4ctx_free(struct c4ctx *self) {
  assert(self->tries.next == &self->tries);
}
