#include "ctx.h"

struct c4ctx *c4ctx_init(struct c4ctx *self) {
  c4ls_init(&self->tries);
  return self;
}
