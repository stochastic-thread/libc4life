#ifndef C4CTX_H
#define C4CTX_H

struct c4try;

struct c4ctx {
  struct c4try *try;
};

struct c4ctx *c4ctx_init(struct c4ctx *self);
void c4ctx_free(struct c4ctx *self);

#endif
