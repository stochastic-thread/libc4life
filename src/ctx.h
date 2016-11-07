#ifndef C4LIFE_CTX
#define C4LIFE_CTX

#include "ls.h"

struct c4ctx {
  struct c4ls tries;
};

struct c4ctx *c4ctx_init(struct c4ctx *self);
void c4ctx_free(struct c4ctx *self);

#endif
