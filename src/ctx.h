#ifndef C4LIFE_CTX
#define C4LIFE_CTX

struct c4try;

struct c4ctx {
  struct c4try *try;
};

struct c4ctx *c4ctx_init(struct c4ctx *self);
void c4ctx_free(struct c4ctx *self);

#endif
